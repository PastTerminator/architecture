/*!
 * @copyright   © 2017 UFAM - Universidade Federal do Amazonas.
 *
 * @brief       Interface de programação de aplicações em C++ para GPIO.
 *
 * @file        dsf_GPIO_ocp.cpp
 * @version     1.0
 * @date        30 Julho 2017
 *
 * @section     HARDWARES & SOFTWARES
 *              +board        FRDM-KL25Z da NXP.
 *              +processor    MKL25Z128VLK4 - ARM Cortex-M0+.
 *              +peripheral   GPIO.
 *              +compiler     Kinetis® Design Studio IDE.
 *              +manual       L25P80M48SF0RM, Rev.3, September 2012.
 *              +revisions    Versão (data): Descrição breve.
 *                             ++ 1.0 (6 Julho 2017): Versão inicial.
 *
 * @section     AUTHORS & DEVELOPERS
 *              +institution  Universidade Federal do Amazonas.
 *              +courses      Engenharia da Computação / Engenharia Elétrica.
 *              +teacher      Miguel Grimm <miguelgrimm@gmail.com>
 *              +student      Versão inicial:
 *                             ++ Hamilton Nascimento <hdan_neto@hotmail.com>
 *
 * @section     LICENSE
 *
 *              GNU General Public License (GNU GPL).
 *
 *              Este programa é um software livre; Você pode redistribuí-lo
 *              e/ou modificá-lo de acordo com os termos do "GNU General Public
 *              License" como publicado pela Free Software Foundation; Seja a
 *              versão 3 da licença, ou qualquer versão posterior.
 *
 *              Este programa é distribuído na esperança de que seja útil,
 *              mas SEM QUALQUER GARANTIA; Sem a garantia implícita de
 *              COMERCIALIZAÇÃO OU USO PARA UM DETERMINADO PROPÓSITO.
 *              Veja o site da "GNU General Public License" para mais detalhes.
 *
 * @htmlonly    http://www.gnu.org/copyleft/gpl.html
 */

#include <MKL25Z4.h>
#include "dsf_GPIO_ocp.h"

/*!
 *   @fn       dsf_GPIO_ocp
 *
 *   @brief    Método construtor da classe.
 *
 *   Este é o método construtor padrão da classe e realiza a associação
 *   entre o objeto de software e o periférico de hardware, bem como a
 *   habilitação do clock do periférico e dos registradores a ele associados.
 *
 *   @param[in]  GPIOName - GPIO;
 *               pin - pino do GPIO selecionado.
 *
 *   @remarks  Siglas e páginas do Manual de Referência KL25:
 *             - PDOR: Port Data Output Register.Pág. 775.
 *             - PDIR: Port Data Input Register.Pág. 777.
 *             - PDDR: Port Direct Input Register. Pág. 778.
 *             - PTOR: Port Toogle Output Register.Pág.777.
 *             - PortxPCRn: Pin Control Register.Pág. 183 (Mux) and 185 (Pull).
 */

dsf_GPIO_ocp::dsf_GPIO_ocp(GPIO_t::dsf_GPIO GPIOName, GPIO_t::dsf_Pin pin) {
  pinPort = 1 << pin;
  bindPeripheral(GPIOName, pin);
  enableModuleClock(GPIOName);
  selectMuxAlternative();
}

/*!
 *   @fn         setPortMode
 *
 *   @brief      Seleciona o modo de operação de um pino.
 *
 *   Este seleciona o modo de operação (entrada ou saída) ao pino
 *   escolhido no construtor.
 *
 *   @param[in]  mode - modo de operação.
 *                      Os modos de operação disponíveis são:
 *                      PortMode_t::Input para modo entrada e
 *                      PortMode_t::Output para modo saída.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - PDDR: Port Direct Input Register. Pág. 778.
 */
void dsf_GPIO_ocp::setPortMode(PortMode_t::dsf_PortMode mode) {
  if (mode == PortMode_t::Input) {
    *addressPDDR &= ~pinPort;
  } else {
    *addressPDDR |= pinPort;
  }
}

/*!
 *   @fn         setPullResistor
 *
 *   @brief      Ajusta resistor de pull.
 *
 *   Este método define se o pino de entrada possui resistor de pull up
 *   ou nenhum resistor ligado internamente.
 *
 *   @param[in]  pullResistor - resistor de pull up ou nenhum.
 *               pullResistor pode ser ajustado como:
 *               PullResistor_t::PullUpResistor,para resistor de pull up.
 *               PullResistor_t::PullNoneResistor,para nenhum resistor de pull.
 *
 *   @remarks    Sigla e pagina do Manual de Referencia KL25:
 *               - PortxPCRn: Pin Control Register. Pág. 183 (Mux) and 185 (Pull).
 */
void dsf_GPIO_ocp::setPullResistor(PullResistor_t::dsf_PullResistor pull) {
  *addressPortxPCRn &= ~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK);
  *addressPortxPCRn |= pull;
}

/*!
 *   @fn         writeBit
 *
 *   @brief      Escreve no pino de saída.
 *
 *   Este método escreve o valor do bit do parâmetro "bit" no pino da
 *   porta de saída.
 *
 *   @param[in]  bit - O valor do bit a ser escrito no pino da porta de saída.
 *
 *   @remarks    Siglas e páginas do Manual de Referência KL25:
 *               - PDOR: Port Data Output Register. Pág. 775.
 */
void dsf_GPIO_ocp::writeBit(int bit) {
  if (bit) {
    *addressPDOR |= pinPort;
  } else {
    *addressPDOR &= ~pinPort;
  }
}

/*!
 *   @fn       readBit
 *
 *   @brief    Lê o pino de entrada.
 *
 *   Este método lê o valor do bit presente no pino da porta de entrada.
 *
 *	 @return   O valor do bit presente no pino da porta de entrada.
 *
 *   @remarks  Siglas e páginas do Manual de Referência KL25:
 *             - PDIR: Port Data Input Register. Pág. 777.
 */
int dsf_GPIO_ocp::readBit() {
  if (*addressPDIR & pinPort) {
    return 1;
  }
  return 0;
}

/*!
 *   @fn       toogleBit
 *
 *   @brief    Altera o bit de saída para o seu complemento.
 *
 *   Este método altera o bit de saída para o seu complemento, i.e.,
 *   se o bit atual é '0', muda para '1' e vice-versa.
 *
 *   @remarks  Siglas e páginas do Manual de Referência KL25:
 *             - PTOR: Port Toogle Output Register.Pág.777.
 */
void dsf_GPIO_ocp::toogleBit() {
  *addressPTOR |= pinPort;
}

/*!
 *   @fn       bindPeripheral
 *
 *   @brief    Associa o objeto de software ao periférico de hardware.
 *
 *   Este método associa os ponteiros aos seus respectivos registradores.
 *
 *   @remarks  Siglas e páginas do Manual de Referência KL25:
 *             - PDOR: Port Data Output Register.Pág. 775.
 *             - PDIR: Port Data Input Register.Pág. 777.
 *             - PDDR: Port Direct Input Register. Pág. 778.
 *             - PTOR: Port Toogle Output Register.Pág.777.
 *             - PortxPCRn: Pin Control Register.Pág. 183 (Mux) and 185 (Pull).
 */
void dsf_GPIO_ocp::bindPeripheral(uint8_t GPIONumber, uint8_t pinNumber) {
  uint32_t baseAddress;

  /*!
   * Cálculo do endereço base do GPIO do parâmetro "GPIOBaseAddress".
   * Address(hexa): GPIOA=400FF014 B=400FF054 C=400FF094 D=400FF0D4 E=400FF114.
   * GPIOBaseAddress = 0x400FF000 (Base GPIOA) + 0x40*(0,1,2,3 ou 4) (Offset).
   *
   */
  baseAddress = GPIOA_BASE + (uint32_t)(0x40*GPIONumber);

  /*!
   * Cálculo do endereço absoluto do PDOR para o GPIO.
   * Address(hexa): GPIOA=400FF014 B=400FF054 C=400FF094 D=400FF0D4 E=400FF114.
   * addressPDOR = address base (Base) + 0x00 (Offset).
   */
  addressPDOR = (volatile uint32_t *)(baseAddress + 0x0);

  /*!
   * Cálculo do endereço absoluto do PDIR para o GPIO.
    * Address(hexa): GPIOA=400FF014 B=400FF054 C=400FF094 D=400FF0D4 E=400FF114.
   * addressPDIR = address base (Base) + 0x10 (Offset).
   */
  addressPDIR = (volatile uint32_t *)(baseAddress + 0x10);

  /*!
   * Cálculo do endereço absoluto do PDDR para o GPIO.
    * Address(hexa): GPIOA=400FF014 B=400FF054 C=400FF094 D=400FF0D4 E=400FF114.
   * addressPDDR = address base (Base) + 0x14 (Offset).
   */
  addressPDDR = (volatile uint32_t *)(baseAddress + 0x14);

  /*!
   * Cálculo do endereço absoluto do PTOR para o GPIO.
   * Address(hexa): GPIOA=400FF014 B=400FF054 C=400FF094 D=400FF0D4 E=400FF114.
   * addressPTOR = address base (Base) + 0xC (Offset).
   */
  addressPTOR = (volatile uint32_t *)(baseAddress + 0xC);

  /*!
   * Cálculo do endereço absoluto do PCR para o GPIO.
   * Address(hexa): GPIOA=400FF014 B=400FF054 C=400FF094 D=400FF0D4 E=400FF114.
   * addressPortxPCRn = 0x40049000 (Base) + 0x1000*(0,1,2,3,4 ou 5)
   *                    + 4*(0,1,2,3,4,...,30) (Offset).
   */
  addressPortxPCRn = (volatile uint32_t *)(0x40049000
                       + 0x1000*GPIONumber
                       + 4*pinNumber);
}

/*!
 *   @fn       enablePeripheralClock
 *
 *   @brief    Habilita o clock do GPIO do periférico.
 *
 *   Este método habilita o clock do GPIO selecionado através do registrador
 *   SIM_SCGC5.
 *
 *   @remarks  Siglas e páginas do Manual de Referência KL25:
 *             - SIM_SCGC5:System Clock Gating Control Register.Pág. 206.
 */
void dsf_GPIO_ocp::enableModuleClock(uint8_t GPIONumber) {
  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK << GPIONumber;
}

/*!
 *   @fn       selectMuxAlternative
 *
 *   @brief    Seleciona o modo GPIO de operação do pino.
 *
 *   Este método seleciona o modo de operação do pino através do registrador
 *   Portx_PCRn, utilizando a macro de configuração PORT_PCR_MUX(x), disponível
 *   no arquivo MKL25Z4.h
 *
 *   @remarks  Siglas e páginas do Manual de Referência KL25:
 *             - PortxPCRn: Pin Control Register.Pág. 183 (Mux) and 185 (Pull).
 */
void dsf_GPIO_ocp::selectMuxAlternative() {
  *addressPortxPCRn = PORT_PCR_MUX(1);
}
