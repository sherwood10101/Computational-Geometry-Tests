# See here for image contents: https://github.com/microsoft/vscode-dev-containers/tree/v0.195.0/containers/cpp/.devcontainer/base.Dockerfile
# [Choice] Debian / Ubuntu version (use Debian 11/9, Ubuntu 18.04/21.04 on local arm64/Apple Silicon): debian-11, debian-10, debian-9, ubuntu-21.04, ubuntu-20.04, ubuntu-18.04
ARG VARIANT=ubuntu-20.04
FROM mcr.microsoft.com/devcontainers/cpp:0-${VARIANT}

# Dependencies of computational_geometry_template app.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends gdb cmake g++ libpng-dev libx11-dev libgl-dev libegl-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libdrm-dev libjpeg-dev

# Additional dependencies of tool_path_test app.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y install --no-install-recommends libboost-all-dev
