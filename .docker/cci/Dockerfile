FROM rockylinux:9
ENV CC=clang
ENV CXX=clang++
ENV SYSTEMC_HOME=/usr/local/systemc
ENV SYSTEMC_LIB_DIR=${SYSTEMC_HOME}/lib-linux64
ENV SYSTEMC_INC_DIR=${SYSTEMC_HOME}/include
ENV LD_LIBRARY_PATH=${SYSTEMC_LIB_DIR}
ENV VER=1.0.1
ENV PREFIX=/usr/local/cci
COPY --from=systemc:build ${SYSTEMC_HOME}/ ${SYSTEMC_HOME}/
RUN dnf -y install clang git make automake autoconf \
    && dnf -y --enablerepo=devel install libtool \
    && dnf -y --enablerepo=devel install rapidjson-devel \
    && dnf clean all \
    && git clone https://github.com/accellera-official/cci.git \
    && cd cci \
    && git switch v$VER --detach \
    && ./config/bootstrap \
    && mkdir objdir \
    && cd objdir \
    && ../configure --prefix=${PREFIX}- --with-systemc=${SYSTEMC_HOME} \
        --with-json=/usr --with-unix-layout --includedir=${PREFIX}/include --libdir=${PREFIX}/lib-linux64 \
    && make -j`nproc` \
    && make check \
    && make install \
    && cd ../../ \
    && rm -rf cci
