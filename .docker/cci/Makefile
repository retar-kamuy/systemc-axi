DOCKER		 = docker

CONTAINER	 = cci

BUILD_IMAGE	 = $(CONTAINER):build

BUILD_FLAGS	 = --rm
BUILD_FLAGS	+= --tag $(BUILD_IMAGE)

RUN_FLAGS	 = --interactive --tty
RUN_FLAGS	+= --detach
RUN_FLAGS	+= --name $(CONTAINER)

###############################################################################
# Rules
###############################################################################
all: build

build:
	@echo Building $(BUILD_IMAGE)
	$(DOCKER) build $(BUILD_FLAGS) .
