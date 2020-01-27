# uart_sniffer
Úlohou je odchytávanie obojsmernej UART komunikácie medzi dvomi ľubovoľnými zariadeniami pomocou STM32. Dáta odchytávanej komunikácie budú následne zapísané na SD kartu pomocou dostupných knižníc. Pre zápis odchytených dát na SD kartu použijeme zbernicu SPI. Odchytené dáta budú zapísané vo formáte CSV. Na SD karte sa bude nachádzať súbor pre konfiguráciu UART periférii.
### Úlohy:
- [x]	UART komunikácie medzi dvomi ľubovoľnými zariadeniami
2.	Fyzické pripojenie STM32 k existujúcej UART komunikácii ako data sniffer
3.	Získanie dát z existujúcej komunikácie
4.	Zápis získaných dát na SD kartu pomocou zbernice SPI vo zvolenom formáte
 
