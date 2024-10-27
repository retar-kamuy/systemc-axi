FROM rockylinux:9
ENV CC=clang
ENV CXX=clang++
ENV VER=1.0.1
ENV PREFIX=/usr/local/iverilog
RUN dnf -y install git autoconf make clang bison flex python3-devel \
    && dnf -y install gperf \
    && dnf -y install epel-release \
    && dnf -y install gtkwave \
    && dnf clean all \
    && git clone https://github.com/steveicarus/iverilog.git \
    && cd iverilog \
    && autoconf \
    && ./configure --prefix=${PREFIX}\
    && make -j`nproc` \
    && make install \
    && make check \
    && cd .. \
    && rm -rf iverilog \
    && dnf -y install python3.12 python3.12-pip \
    && alternatives --set python3 /usr/bin/python3.12 \
    && pip install cocotb pytest cocotbext-axi cocotbext-pcie cocotb-test \
    && dnf clean all
ENV PATH ${PREFIX}/bin:${PATH}
