# Use the Ubuntu 23
FROM ubuntu:23.04

# Update the package lists for upgrades and new package installations
RUN apt-get update && apt-get install -y \
  build-essential \
  cmake \
  git \
  curl \
  zsh \
  libboost-all-dev \
  nodejs \
  npm \
  libasio-dev \
  nlohmann-json3-dev \
  libmysqlclient-dev \
  mysql-client \
  g++ \
  make \
  libpq-dev \
  libsqlite3-dev \
  libhiredis-dev \
  libssl-dev \
  redis-tools \
  openssl 

# Install redis-plus-plus from source
RUN git clone https://github.com/sewenew/redis-plus-plus.git && \
  cd redis-plus-plus && \
  mkdir build && \
  cd build && \
  cmake .. && \
  make && \
  make install && \
  cd ../.. && \
  rm -rf redis-plus-plus

# Add jwt-cpp
RUN git clone https://github.com/Thalhammer/jwt-cpp.git && \
  cp -r jwt-cpp/include/* /usr/local/include/ && \
  rm -rf jwt-cpp

# Install oh-my-zsh
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

# Set the working directory to /app
WORKDIR /app

# Clone Crow repository
RUN git clone https://github.com/CrowCpp/Crow.git && \
  cp -r Crow/include/* /usr/local/include/ && \
  rm -rf Crow

# Copy the current directory contents into the container at /app
COPY . .

# Install nodemon
RUN npm install -g nodemon

# Update library cache
RUN ldconfig

# Generate JWT secret and export it as environment variable
RUN openssl rand -base64 32 > /tmp/jwt_secret && \
    export JWT_SECRET=$(cat /tmp/jwt_secret) && \
    echo "export JWT_SECRET=$(cat /tmp/jwt_secret)" >> /etc/profile.d/jwt.sh && \
    echo "export JWT_SECRET=$(cat /tmp/jwt_secret)" >> /root/.bashrc && \
    rm /tmp/jwt_secret

# Update CMD to use the executable from build directory
CMD ["bash"]