/*!
 * @copyright   � 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Exemplo de uso da classe Delay.
 *
 * @example     main.cpp
 * @version     1.0
 * @date        02 Agosto 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+
 *              +Peripheral   TPM, GPIO e Led RGB.
 *              +compiler     Kinetis� Design Studio IDE
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas
 *              +courses      Engenharia da Computa��o / Engenharia El�trica
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Vers�o inicial:
 *                             ++ Hamilton Nascimento <hdan_neto@hotmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL).
 *
 *              Este programa � um software livre; Voc� pode redistribu�-lo
 *              e/ou modific�-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              vers�o 3 da licen�a, ou qualquer vers�o posterior.
 *
 *              Este programa � distribu�do na esperan�a de que seja �til,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia impl�cita de
 *              COMERCIALIZA��O OU USO PARA UM DETERMINADO PROP�SITO.
 *              Veja o site da "GNU General Public License" para mais detalhes.
 *
 * @htmlonly    http://www.gnu.org/copyleft/gpl.html
 */

#include "dsf_GPIO_ocp.h"
#include "dsf_Delay_ocp.h"

/*! Objeto led verde. */
dsf_GPIO_ocp greenLed(GPIO_t::dsf_GPIOB, GPIO_t::dsf_PTB18);

/*! * Objeto key. */
dsf_GPIO_ocp key(GPIO_t::dsf_GPIOA, GPIO_t::dsf_PTA1);

/*! Objeto da classe delay. */
dsf_Delay_ocp tpm(TPM_t::dsf_TPM2);


void setup() {
	greenLed.setPortMode(PortMode_t::Output);
	key.setPortMode(PortMode_t::Input);
	key.setPullResistor(PullResistor_t::PullUpResistor);
	tpm.setFrequency(TPMDiv_t::Div128);
}

int main() {
  int contaPiscas = 0;
  int bit = 0;

  setup();
  while (true) {
    /*! Aguarda 400 ms. */
    tpm.waitDelay(0xFFFF);
    if (key.readBit()) {
    	bit = !bit;
    	greenLed.writeBit(bit);
    	contaPiscas++;
    } else {
    	greenLed.writeBit(1);
    }
  }
  return 0;
}
