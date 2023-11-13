FROM        debian:buster

LABEL       authors="tnoulens"

RUN	        apt update && \
            apt upgrade -y && \
            apt install -y curl \
            fish \
            make \
            usermode \
            gcc \
            gcc-multilib \
            gdb \
            valgrind \
            dsniff \
            arp \
            net-tools

COPY        . .

WORKDIR     ./ft_malcolm

RUN        make
