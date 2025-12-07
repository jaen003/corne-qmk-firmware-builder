FROM debian:13.1-slim
ENV PATH="/home/vscode/.local/bin:${PATH}"
RUN apt-get update && apt-get install -y --no-install-recommends git make gcc-avr avr-libc \
        binutils-avr avrdude dfu-programmer dfu-util python3 python3-pip \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*
RUN groupadd -g 1000 vscode \
 && useradd -u 1000 -g vscode -m -s /bin/bash vscode \
 && echo 'vscode ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
USER vscode
RUN pip install --no-cache-dir --break-system-packages qmk==1.1.8
RUN qmk setup -y
RUN mkdir -p /home/vscode/qmk_firmware/keyboards/crkbd/keymaps/custom
WORKDIR /workspaces