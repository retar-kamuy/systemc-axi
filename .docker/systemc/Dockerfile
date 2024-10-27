FROM rockylinux:9
ENV CC=clang
ENV CXX=clang++
ENV VER=1.0.1
ENV PREFIX=/usr/local/systemc
RUN dnf -y install clang git make automake autoconf \
    && dnf -y --enablerepo=devel install libtool \
    && dnf -y clean all \
    && git clone https://github.com/accellera-official/systemc.git \
    && cd systemc \
#    && git switch ${VER} --detach \
    && ./config/bootstrap \
    && mkdir objdir \
    && cd objdir \
    && ../configure --prefix=${PREFIX} --enable-pthreads \
    && make -j`nproc` \
    && make check \
    && make install \
    && cd ../../ \
    && rm -rf systemc
ENV SYSTEMC_HOME=${PREFIX}
ENV SYSTEMC_INCLUDE=${SYSTEMC_HOME}/include
ENV SYSTEMC_LIBDIR=${SYSTEMC_HOME}/lib-linux64
ENV LD_LIBRARY_PATH=${SYSTEMC_LIB_DIR}