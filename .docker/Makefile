DOCKER		 = docker

CONTAINER	 = verilog-ethernet

BUILD_IMAGE	 = $(CONTAINER):latest

BUILD_FLAGS	 = --rm
BUILD_FLAGS	+= --tag $(BUILD_IMAGE)
BUILD_FLAGS	+= -f ./.docker/Dockerfile

RUN_FLAGS	 = --interactive --tty
RUN_FLAGS	+= --detach
RUN_FLAGS	+= --name $(CONTAINER)

###############################################################################
# Rules
###############################################################################
all: build run

build_systemc:
	$(DOCKER) build --rm --tag systemc:build . -f .docker/systemc/Dockerfile

build_cci:
	$(DOCKER) build --rm --tag cci:build . -f .docker/cci/Dockerfile

build_verilator:
	$(DOCKER) build --rm --tag verilator:build . -f .docker/verilator/Dockerfile

build: build_systemc build_cci build_verilator
	@echo Building $(CONTAINER):latest
	$(DOCKER) build $(BUILD_FLAGS) .
	$(DOCKER) image rm verilator:build
	$(DOCKER) image rm cci:build
	$(DOCKER) image rm systemc:build
	$(DOCKER) image rm rockylinux:9

run:
	$(DOCKER) run $(RUN_FLAGS) $(BUILD_IMAGE)

clean:
	$(DOCKER) rm -f $(CONTAINER)
	$(DOCKER) image rm $(BUILD_IMAGE)
