FROM debian:13.1-slim
ARG USERNAME=vscode
ARG UID=1000
ARG GID=1000
ARG QMK_HOME=/home/${USERNAME}/qmk_firmware
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt-get update \
    && apt-get install -y --no-install-recommends git openssh-client make gcc-avr avr-libc \
        binutils-avr avrdude dfu-programmer dfu-util python3 python3-pip
RUN groupadd -g ${GID} ${USERNAME} \
    && useradd -m -u ${UID} -g ${GID} -s /bin/bash ${USERNAME} \
    && echo "${USERNAME} ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers
USER ${USERNAME}
ENV PATH="/home/${USERNAME}/.local/bin:${PATH}"
RUN git clone --depth=1 https://github.com/qmk/qmk_firmware.git ${QMK_HOME}
RUN --mount=type=cache,target=/home/${USERNAME}/.cache/pip,uid=${UID},gid=${GID} \
    pip install --break-system-packages qmk==1.1.8
RUN qmk setup -y --home /home/${USERNAME}/qmk_firmware
RUN mkdir -p /home/${USERNAME}/qmk_firmware/keyboards/crkbd/keymaps/custom