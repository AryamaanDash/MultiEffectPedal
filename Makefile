# Project Name
TARGET = MultiEffectPedal

# Sources
CPP_SOURCES = \
	src/main.cpp \
	src/AudioEngine.cpp \
	src/EffectManager.cpp \
	src/PedalControls.cpp \
	src/effects/BypassEffect.cpp \
	src/effects/DelayEffect.cpp \
	src/effects/DistortionEffect.cpp \
	src/effects/FlangerEffect.cpp \
	src/effects/ReverbEffect.cpp

C_INCLUDES += -Iinclude

USE_DAISYSP_LGPL = 1


# Library Locations
LIBDAISY_DIR = ../../libDaisy/
DAISYSP_DIR = ../../DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
