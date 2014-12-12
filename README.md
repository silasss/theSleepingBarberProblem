The Sleeping Barber Problem
========================

This is a example of an implementation of sleeping barber problem, written in C language.

This algorithm implement threads and semaphore (-pthreads).

This algorithm use normal distribution (math.h - param -lm) for the arrival of clients, in a range of 1.0 - 3.0 seconds for interations.



- Technical specifications

  SO: Debian GNU/Linux jessie/sid
  IDE: Netbeans 8.0
  Compiler: gcc (Debian 4.8.2-16) 4.8.2
  Params of compilation (in IDE): -lpthread -lm
  Language fo software: pt-BR (portuguese brazilian)




- Functions

  int main();
  
  void* pthBarbeiro();
  
  void* pthCliente(int *param);
  
  double distNormalAleatoria();
  
  void cabecalho();
  



- Include libs

  &#35;include &lt;stdio.h&gt;
  
  &#35;include &lt;stdlib.h&gt;
  
  &#35;include &lt;unistd.h&gt;
  
  &#35;include &lt;math.h&gt;
  
  &#35;include &lt;pthread.h&gt;
  
  &#35;include &lt;semaphore.h&gt;
  
