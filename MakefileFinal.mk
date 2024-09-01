CC = gcc
FLAGS = -std=c99 -g3
LIBS = -lm

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))

all: $(TARGET)

vpath %.c $(dir $(C_SOURCES))

$(BUILD_DIR)/%.o: %.c
	$(CC) $(FLAGS) $(CFLAGS) -MD $(C_INCLUDES) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $(TARGET) $(LIBS)

.PHONY = start
start: $(TARGET)
	./$(TARGET)

.PHONY = clean
clean:
	rm -f $(BUILD_DIR)/*

-include $(OBJECTS:.o=.d)