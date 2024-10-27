FROM rockylinux:9
ARG VER=5.026
ENV CC=clang
ENV CXX=clang++
ENV PREFIX=/usr/local/verilator
ENV SYSTEMC_HOME=/usr/local/systemc
ENV SYSTEMC_INCLUDE=${SYSTEMC_HOME}/include
ENV SYSTEMC_LIBDIR=${SYSTEMC_HOME}/lib-linux64
ENV LD_LIBRARY_PATH=${SYSTEMC_LIB_DIR}
ENV CCI_HOME=/usr/local/cci
COPY --from=systemc:build ${SYSTEMC_HOME}/ ${SYSTEMC_HOME}/
COPY --from=cci:build ${CCI_HOME}/ ${CCI_HOME}/
RUN dnf -y install clang \
    && dnf -y install git perl python3 make autoconf flex bison \
    && dnf -y install epel-release \
    && dnf -y install ccache \
    && dnf -y --enablerepo=crb install help2man \
    && dnf -y install gperftools-devel numactl \
    && dnf -y install zlib zlib-devel \
    && dnf clean all \
    && git clone https://github.com/verilator/verilator \
    && cd verilator \
    && git switch v${VER} --detach \
    && autoconf \
    && ./configure --prefix=${PREFIX} \
    && make -j`nproc` \
    && make install \
    && cd .. \
    && rm -rf verilator
ENV VERILATOR_ROOT=${PREFIX}
