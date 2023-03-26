/****************************************************************************
http://retro.moe/unijoysticle2

Copyright 2022 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#ifndef UNI_PROPERTY_MEM_H
#define UNI_PROPERTY_MEM_H

#include "uni_property.h"

void uni_property_mem_init(void);
void uni_property_mem_set(const char* key, uni_property_type_t type, uni_property_value_t value);
uni_property_value_t uni_property_mem_get(const char* key, uni_property_type_t type, uni_property_value_t def);

#endif  // UNI_PROPERTY_MEM_H
