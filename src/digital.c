/* Copyright 2022, Lino Fabian Condorì <condori.lino.f@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** @file digital.c
 **
 ** @brief Plantilla de archivos fuente
 **
 ** Plantilla para los archivos de codigo fuente de prácticos de las 
 ** asignaturas Diseño Integrado de Sistemas Emebebidos y Sistemas Embebidos
 ** de Tiempo Real dictadas en de la Especialización en Integración de
 ** Sistemas Informaticos de la Univesidad Nacional de Tucumán
 ** 
 ** | RV | YYYY.MM.DD | Autor       | Descripción de los cambios              |
 ** |----|------------|-------------|-----------------------------------------|
 ** |  1 | 2022.09.10 | lcondori    | Version inicial del archivo             |
 ** 
 ** @defgroup plantilla Plantillas de Archivos
 ** @brief Capa de Hardware de abtraccion
 ** @{ 
 */

/* === Inclusiones de cabeceras ============================================ */
#include "digital.h"
#include "chip.h"

/* === Definicion y Macros privados ======================================== */
#ifndef INPUT_INSTANCES
    #define INPUT_INSTANCES    4
#endif

#ifndef OUTPUT_INSTANCES
    #define OUTPUT_INSTANCES    8
#endif
/* === Declaraciones de tipos de datos privados ============================ */
struct digital_output_s
{
    uint8_t gpio;
    uint8_t bit;
    bool allocated;
};

struct digital_input_s
{
    uint8_t gpio;
    uint8_t bit;
    bool allocated;
    bool inverted;
    bool last_state;
};

/* === Definiciones de variables privadas ================================== */
static struct digital_output_s instances[OUTPUT_INSTANCES] = {0};
static struct digital_input_s Instances[INPUT_INSTANCES] = {0};


/* === Definiciones de variables publicas ================================== */

/* === Declaraciones de funciones privadas ================================= */

/* === Definiciones de funciones privadas ================================== */

digital_output_t DigitalOutputAllocate(void){
    digital_output_t output = NULL;
    for(int index=0; index <OUTPUT_INSTANCES; index++){
        if(instances[index].allocated == false){
            instances[index].allocated = true;
            output = &instances[index];
            break;
        }
    }
    return output;
}

digital_input_t DigitalInputAllocate(void){
    digital_input_t input = NULL;
    for(int index=0; index <OUTPUT_INSTANCES; index++){
        if(Instances[index].allocated == false){
            Instances[index].allocated = true;
            input = &Instances[index];
            break;
        }
    }
    return input;
}

/* === Definiciones de funciones publicas ================================== */
digital_output_t DigitalOutputCreate(uint8_t gpio, uint8_t bit){
    digital_output_t output = DigitalOutputAllocate();
    if (output){
        output->gpio = gpio;
        output->bit = bit;
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpio, bit, false);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, gpio, bit, true);
    }
    return output;
}
void DigitalOutputActivate(digital_output_t output){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->gpio, output->bit, true);

}
void DigitalOutputDesactivate(digital_output_t output){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->gpio, output->bit, false);
}
void DigitalOutputToggle(digital_output_t output){
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, output->gpio, output->bit);
}

digital_input_t DigitalInputCreate(uint8_t gpio, uint8_t bit, bool inverted){
    digital_input_t input = DigitalInputAllocate();
    if (input){
        input->gpio = gpio;
        input->bit = bit;
        input->inverted = inverted;
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, gpio, bit, false);
    }
    return input;
}

bool DigitalInputGetState(digital_input_t input){
    return Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, input->gpio, input->bit);
}

bool DigitalInputHasActivated(digital_input_t input){
    bool state = DigitalInputGetState(input);
    bool result = state && !input->last_state;
    input->last_state = state;
    return result;
}

bool DigitalInputHasDesactivated(digital_input_t input){
    bool state = DigitalInputGetState(input);
    bool result = !state && !input->last_state;
    input->last_state = state;
    return result;
}

bool DigitalInputHasChanged(digital_input_t input){
    bool state = DigitalInputGetState(input);
    bool result = state != input->last_state;
    input->last_state = state;
    return result;
}
/* === Ciere de documentacion ============================================== */

/** @} Final de la definición del modulo para doxygen */

