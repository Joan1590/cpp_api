#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>

namespace Export
{
  using json = nlohmann::json;

  class DataExporter
  {
  public:
    // Export JSON array to CSV string
    static std::string toCSV(const json &data, const std::vector<std::string> &columns = {})
    {
      if (!data.is_array() || data.empty())
      {
        return "";
      }

      std::stringstream ss;
      std::vector<std::string> cols = columns;

      // If no columns specified, use keys from first object
      if (cols.empty() && data[0].is_object())
      {
        for (auto it = data[0].begin(); it != data[0].end(); ++it)
        {
          cols.push_back(it.key());
        }
      }

      // Write header row
      for (size_t i = 0; i < cols.size(); ++i)
      {
        ss << escapeCSV(cols[i]);
        if (i < cols.size() - 1)
          ss << ",";
      }
      ss << "\n";

      // Write data rows
      for (const auto &row : data)
      {
        for (size_t i = 0; i < cols.size(); ++i)
        {
          if (row.contains(cols[i]))
          {
            ss << escapeCSV(valueToString(row[cols[i]]));
          }
          if (i < cols.size() - 1)
            ss << ",";
        }
        ss << "\n";
      }

      return ss.str();
    }

    // Export JSON array to HTML table string
    static std::string toHTML(const json &data, const std::vector<std::string> &columns = {}, const std::string &tableClass = "data-table")
    {
      if (!data.is_array() || data.empty())
      {
        return "<table></table>";
      }

      std::stringstream ss;
      std::vector<std::string> cols = columns;

      // If no columns specified, use keys from first object
      if (cols.empty() && data[0].is_object())
      {
        for (auto it = data[0].begin(); it != data[0].end(); ++it)
        {
          cols.push_back(it.key());
        }
      }

      ss << "<table class=\"" << tableClass << "\">\n";

      // Write header row
      ss << "  <thead>\n    <tr>\n";
      for (const auto &col : cols)
      {
        ss << "      <th>" << escapeHTML(col) << "</th>\n";
      }
      ss << "    </tr>\n  </thead>\n";

      // Write data rows
      ss << "  <tbody>\n";
      for (const auto &row : data)
      {
        ss << "    <tr>\n";
        for (const auto &col : cols)
        {
          ss << "      <td>";
          if (row.contains(col))
          {
            ss << escapeHTML(valueToString(row[col]));
          }
          ss << "</td>\n";
        }
        ss << "    </tr>\n";
      }
      ss << "  </tbody>\n</table>";

      return ss.str();
    }

    // Export JSON array to XML string
    static std::string toXML(const json &data, const std::string &rootElement = "data", const std::string &itemElement = "item")
    {
      if (!data.is_array())
      {
        return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<" + rootElement + "></" + rootElement + ">";
      }

      std::stringstream ss;
      ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
      ss << "<" << rootElement << ">\n";

      for (const auto &row : data)
      {
        ss << "  <" << itemElement << ">\n";
        if (row.is_object())
        {
          for (auto it = row.begin(); it != row.end(); ++it)
          {
            ss << "    <" << it.key() << ">" << escapeXML(valueToString(it.value())) << "</" << it.key() << ">\n";
          }
        }
        ss << "  </" << itemElement << ">\n";
      }

      ss << "</" << rootElement << ">";
      return ss.str();
    }

    // Export to plain text table format
    static std::string toTextTable(const json &data, const std::vector<std::string> &columns = {})
    {
      if (!data.is_array() || data.empty())
      {
        return "";
      }

      std::vector<std::string> cols = columns;

      // If no columns specified, use keys from first object
      if (cols.empty() && data[0].is_object())
      {
        for (auto it = data[0].begin(); it != data[0].end(); ++it)
        {
          cols.push_back(it.key());
        }
      }

      // Calculate column widths
      std::vector<size_t> widths(cols.size());
      for (size_t i = 0; i < cols.size(); ++i)
      {
        widths[i] = cols[i].length();
      }

      for (const auto &row : data)
      {
        for (size_t i = 0; i < cols.size(); ++i)
        {
          if (row.contains(cols[i]))
          {
            widths[i] = std::max(widths[i], valueToString(row[cols[i]]).length());
          }
        }
      }

      std::stringstream ss;

      // Header separator
      auto writeSeparator = [&]()
      {
        ss << "+";
        for (size_t w : widths)
        {
          ss << std::string(w + 2, '-') << "+";
        }
        ss << "\n";
      };

      writeSeparator();

      // Header row
      ss << "|";
      for (size_t i = 0; i < cols.size(); ++i)
      {
        ss << " " << std::setw(widths[i]) << std::left << cols[i] << " |";
      }
      ss << "\n";

      writeSeparator();

      // Data rows
      for (const auto &row : data)
      {
        ss << "|";
        for (size_t i = 0; i < cols.size(); ++i)
        {
          std::string value = "";
          if (row.contains(cols[i]))
          {
            value = valueToString(row[cols[i]]);
          }
          ss << " " << std::setw(widths[i]) << std::left << value << " |";
        }
        ss << "\n";
      }

      writeSeparator();

      return ss.str();
    }

    // Save string content to file
    static bool saveToFile(const std::string &content, const std::string &filepath)
    {
      try
      {
        std::ofstream file(filepath);
        if (!file.is_open())
          return false;
        file << content;
        return true;
      }
      catch (...)
      {
        return false;
      }
    }

    // Get CSV response with proper headers
    static crow::response csvResponse(const json &data, const std::string &filename = "export.csv")
    {
      std::string csv = toCSV(data);
      crow::response res(200, csv);
      res.add_header("Content-Type", "text/csv; charset=utf-8");
      res.add_header("Content-Disposition", "attachment; filename=\"" + filename + "\"");
      return res;
    }

    // Get XML response with proper headers
    static crow::response xmlResponse(const json &data, const std::string &filename = "export.xml")
    {
      std::string xml = toXML(data);
      crow::response res(200, xml);
      res.add_header("Content-Type", "application/xml; charset=utf-8");
      res.add_header("Content-Disposition", "attachment; filename=\"" + filename + "\"");
      return res;
    }

  private:
    // Convert JSON value to string
    static std::string valueToString(const json &value)
    {
      if (value.is_string())
        return value.get<std::string>();
      if (value.is_number_integer())
        return std::to_string(value.get<int64_t>());
      if (value.is_number_float())
        return std::to_string(value.get<double>());
      if (value.is_boolean())
        return value.get<bool>() ? "true" : "false";
      if (value.is_null())
        return "";
      return value.dump();
    }

    // Escape string for CSV
    static std::string escapeCSV(const std::string &str)
    {
      bool needsQuotes = str.find(',') != std::string::npos ||
                         str.find('"') != std::string::npos ||
                         str.find('\n') != std::string::npos;

      if (!needsQuotes)
        return str;

      std::string escaped = "\"";
      for (char c : str)
      {
        if (c == '"')
          escaped += "\"\"";
        else
          escaped += c;
      }
      escaped += "\"";
      return escaped;
    }

    // Escape string for HTML
    static std::string escapeHTML(const std::string &str)
    {
      std::string result;
      for (char c : str)
      {
        switch (c)
        {
        case '&':
          result += "&amp;";
          break;
        case '<':
          result += "&lt;";
          break;
        case '>':
          result += "&gt;";
          break;
        case '"':
          result += "&quot;";
          break;
        case '\'':
          result += "&#39;";
          break;
        default:
          result += c;
        }
      }
      return result;
    }

    // Escape string for XML
    static std::string escapeXML(const std::string &str)
    {
      return escapeHTML(str);
    }
  };
}
