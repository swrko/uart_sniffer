# uart_sniffer
Úlohou je odchytávanie obojsmernej UART komunikácie medzi dvomi ľubovoľnými zariadeniami pomocou STM32. Dáta odchytávanej komunikácie budú následne zapísané na SD kartu pomocou dostupných knižníc. Pre zápis odchytených dát na SD kartu použijeme zbernicu SPI. Odchytené dáta budú zapísané vo formáte CSV. Na SD karte sa bude nachádzať súbor pre konfiguráciu UART periférii.
