FROM rockylinux:9
ENV CC=clang
ENV CXX=clang++
ENV SYSTEMC_HOME=/usr/local/systemc
ENV SYSTEMC_LIB_DIR=${SYSTEMC_HOME}/lib-linux64
ENV SYSTEMC_INC_DIR=${SYSTEMC_HOME}/include
ENV LD_LIBRARY_PATH=${SYSTEMC_LIB_DIR}
ENV CCI_HOME=/usr/local/cci
ENV VERILATOR_ROOT=/usr/local/verilator
WORKDIR /root/
COPY --from=systemc:build ${SYSTEMC_HOME}/ ${SYSTEMC_HOME}/
COPY --from=cci:build ${CCI_HOME}/ ${CCI_HOME}/
COPY --from=verilator:build ${VERILATOR_ROOT}/ ${VERILATOR_ROOT}/
RUN dnf -y install clang \
    && dnf -y install git perl python3 make autoconf flex bison \
    && dnf -y install epel-release \
    && dnf -y install ccache \
    && dnf -y --enablerepo=crb install help2man \
    && dnf -y install gperftools-devel numactl \
    && dnf -y install zlib zlib-devel \
    && dnf -y install python3-pip  \
    && dnf clean all \
    && pip install cpplint
ENV PATH=${VERILATOR_ROOT}/bin:$PATH
