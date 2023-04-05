set(srcs "esp_bt_tank.c")

set(requires "PS4-esp32")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}")
