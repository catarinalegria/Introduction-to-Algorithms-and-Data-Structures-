/*
 * File:  proj1.c
 * Author:  Catarina Alegria
 * Description: projeto 1
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*tamanho maximo que as strings podem tomar (63 caracteres + '/0')*/
#define MAXSTRING 64
/*tamanho maximo de eventos que as 10 salas podem comportar*/
#define DIM_MAX 1000
/*tamanho maximo que um dado input pode ter*/
#define DIM_INPUT 335
/*numero de dados que caracterizam um evento */
#define SIZE_EVENTO 9
/*delimitador entre dados do input fornecido*/
#define DELIM ":"
/*numero maximo de participantes por evento*/
#define MAX_PART 3
/*constante de controlo utilizada na funcao verifica, que permite imprimir, na
funcao A, uma mensagem de erro propria da mesma*/
#define CTRL_A 1
#define CTRL_N_A 0
/*constante com o numero de elementos que caracterizam uma data(ano, dia, mes)*/
#define SIZE_DATE 3

/*estrutura evento, caracterizada pela sua descricao, resposavel e participantes
(arrays de strings); pela data, hora de inicio, hora de fim, sala, duracao e numero de
participantes(inteiros)*/
typedef struct evento{

     char descricao[MAXSTRING],responsavel[MAXSTRING],participantes[MAX_PART][MAXSTRING];
     int data,inicio,fim,sala,n_part,duracao;

} Evento;

/* funcao que verifica se nao ha sobreposicoes de horas entre dois eventos;
recebe as horas iniciais e finais de dois eventos; retorna 1 se houver
sobreposicoes entre as horas*/
int valide_time(int event_ini, int event_end, int time_ini, int time_end){
     return !((event_end<=time_ini) || (time_end<=event_ini));
}

/*funcao que recebe input e coloca-o num array*/
void read_input(char input[DIM_INPUT]){
      char c;
      int i;

      c=getchar();
      for (i=0;c!='\n' && c!=EOF;i++){
          input[i]=c;
          c=getchar();
      }
      input[i]=0;
}

/*funcao que separa em blocos a informacao da string fornecida, sendo o
caracter ':' o delimitador entre os varios blocos.; recebe como argumentos
a string que se pretende separar e a string onde se coloca a string separada*/
void  separate_string(char input[MAXSTRING],char new[SIZE_EVENTO][MAXSTRING]){
      int i;
      char* p;

      i=0;
      for (p = strtok(input,DELIM); p != NULL; p = strtok(NULL, DELIM))
            strcpy(new[i++],p);
    }

/*funcao que verifica se nao ha sobreposicoes entre os eventos agendados e o evento
que se pretende agendar (evento test);recebe dois eventos e uma variavel ctrl, usada
no comando A; retorna o valor 1 se for houver alguma sobreposicao e 0 caso contratrio*/
int verify(Evento event, Evento test, int ctrl){
    int j,i;
    /*variavel que apresenta o valor 1 caso, ao se verificar os participantes e
    responsaveis, haja alguma sobreposicao*/
    int control=0;

    if (event.data==test.data){
      /*compara as salas dos dois eventos*/
      if (event.sala==test.sala)
        if (valide_time(event.inicio,event.fim,test.inicio,test.fim)){
            printf("Impossivel agendar evento %s. Sala%d ocupada.\n", test.descricao,test.sala);
            return 1;
          }

      /*compara o responsavel do evento test com os responsaveis dos eventos agendados*/
      if(strcmp(event.responsavel,test.responsavel)==0)
        if (valide_time(event.inicio,event.fim,test.inicio,test.fim)){
            printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", test.descricao,test.responsavel);
            control=1;
          }

      /*compara os participantes do evento test com os responsaveis dos eventos agendados*/
      for (i=0;i<test.n_part;i++)
          if ((strcmp(event.responsavel,test.participantes[i])==0) && valide_time(event.inicio,event.fim,test.inicio,test.fim)){
              if(ctrl!=1)
                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n",test.descricao,test.participantes[i]);
              else
                /*mensagem de erro retornada no comando A (variavel ctrl igual a 1)*/
                printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n",test.participantes[i]);
              control=1;
          }

      /*compara o responsavel do evento test com os participantes dos eventos agendados*/
      for (i=0;i<event.n_part;i++)
          if ((strcmp(test.responsavel,event.participantes[i])==0) && valide_time(event.inicio,event.fim,test.inicio,test.fim)){
              printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", test.descricao,test.responsavel);
              control=1;
          }

      /*compara os participantes do evento test com os participantes dos eventos agendados*/
      /*ciclo que percorre cada participante do evento test*/
      for (i=0; i<test.n_part; i++)
        /*ciclo que percorre cada participante dos eventos agendados*/
        for (j=0;j<event.n_part;j++)
          if ((strcmp(event.participantes[j],test.participantes[i])==0) && valide_time(event.inicio,event.fim,test.inicio,test.fim)){
              if(ctrl!=1)
                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n",test.descricao,test.participantes[i]);
              else
                /*mensagem de erro retornada no comando A (variavel ctrl igual a 1)*/
                printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n",test.participantes[i]);
              control=1;
          }
      }

      if (!control)
        return 0;

      return 1;
}

/*funcao que separa uma data em ano, mes e dia; recebe um evento e a sua data;
nao retorna nada*/
void split_date(Evento evento, int date[SIZE_DATE]){
  /*o primeiro indice do array corresponde ao ano, o segundo ao mes e o terceiro ao dia da data*/
  date[0]=(evento.data)%10000;
  date[1]=((evento.data)/10000)%100;
  date[2]=(evento.data)/1000000;
}

/*funcao que, dado um evento, imprime no ecra a informacao desse evento; recebe
como parametro um evento; nao retorna nada*/
void write(Evento event){
      int j=0,date[SIZE_DATE];

      split_date(event,date);

      printf("%s %02d%02d%d %02d%02d %d Sala%d %s\n", event.descricao, date[2],
      date[1], date[0], (event.inicio)/100, (event.inicio)%100, event.duracao,
      event.sala,event.responsavel);

      printf("* ");
      /*ciclo que percorre o array com os participantes do evento, ate ao penultimo
      participante*/
      for (j=0;j<event.n_part-1;j++)
          printf("%s ", event.participantes[j]);
      printf("%s\n", event.participantes[j]);

  }


/*funcao que retorna 1 se o primeiro evento for menor que o segundo evento,
e retorna 0 caso contrario*/
int check_date(Evento event1, Evento event2){
    int date1[SIZE_DATE],date2[SIZE_DATE];

    split_date(event1,date1);
    split_date(event2,date2);

    /* verifica o ano*/
    if (date1[0]<date2[0])
        return 1;

    /* para o mesmo ano, verifica o mes*/
    else if ((date1[0]==date2[0])&&(date1[1]<date2[1]))
        return 1;

    /* para o mesmo ano e mes,verifica o dia*/
    else if ((date1[0]==date2[0])&&(date1[1]==date2[1])&&(date1[2]<date2[2]))
        return 1;

    return 0;
}

  /*funcao que dados a hora inicial e a duracao, retorna a hora final do evento*/
  int calculate_end(int inicial, int duration){
      int end;

      end=(inicial/100)*60 + inicial%100 + duration;
      /*acerta a hora final, caso esta exceda os 60 minutos*/
      end=(end/60)*100+end%60;

      return end;
  }

/*funcao que ordena cronologicamente e imprime no ecra um array; recebe como
parametros um array de eventos e a sua dimensao; nao retorna nada*/
void sort(Evento vec[DIM_MAX], int size){
      int i,j;
      /*variavel que, ao percorrer o array de eventos, contem o indice do evento
      mais recente*/
      int min;
      Evento aux;

      for (i=0;i<size;i++){
          min=i;
          for (j=i+1;j<size;j++){

            /*verifica se a data do evento seguinte e menor que a data do evento anterior do array*/
            if ((vec[j].data!=vec[min].data)&&(check_date(vec[j],vec[min])))
                min=j;
            /*verifica se, para a mesma data, a hora de inicio do evento seguinte
            e menor que a hora de inicio do evento anterior do array*/
            else if ((vec[j].data==vec[min].data)&&(vec[j].inicio<vec[min].inicio))
                    min=j;
            /*verifica se, para a mesma data e hora de inicio, a sala do evento
            seguinte e menor que a sala do evento anterior do array*/
            else if ((vec[j].data==vec[min].data)&&(vec[j].inicio==vec[min].inicio)&&(vec[j].sala<vec[min].sala))
                      min=j;
            }

            aux=vec[min];
            vec[min]=vec[i];
            vec[i]=aux;
        }

      for(i=0;i<size;i++)
          write(vec[i]);
    }

/*funcao que adiciona um novo evento a lista de eventos, caso este seja
valido;recebe como parametros o array com os eventos agendados e a sua dimensao;
retorna dimensao do array*/
int  addEvent( Evento list[DIM_MAX],int size) {

      char input[DIM_INPUT],c;
      /* array de strings que ira conter os participantes do evento*/
      char participantes[MAX_PART][MAXSTRING];
      char new[SIZE_EVENTO][MAXSTRING];
      int i,date,inicial,duration,end,room,n_part;
      /*variavel com valor 1 caso nao seja possivel agendar o evento*/
      int control=0;
      /*variavel que contem o numero de caracteres ':' encontrados ao ler o input*/
      int cont=0;

      c=getchar();
      for (i=0;c!='\n' && c!=EOF;i++){
          input[i]=c;
          if (c==':')
            cont++;
          c=getchar();
      }
      input[i]=0;

      separate_string(input,new);

      date=atoi(new[1]);
      inicial=atoi(new[2]);
      duration=atoi(new[3]);
      room=atoi(new[4]);
      end=calculate_end(inicial,duration);

      /*subtrai 5 ao numero de caracteres ':' que o input contem, obtendo-se
      o numero de participantes do evento*/
      n_part=cont-5;
      for(i=0;i<n_part;i++)
        strcpy(participantes[i],new[6+i]);

      list[size].n_part=n_part;
      strcpy(list[size].descricao,new[0]);
      list[size].data=date;
      list[size].duracao=duration;
      list[size].sala=room;
      list[size].fim=end;
      list[size].inicio=inicial;
      strcpy(list[size].responsavel,new[5]);

      for(i=0;i<MAX_PART;i++)
        strcpy(list[size].participantes[i],participantes[i]);

      /*ciclo que verifica se ha sobreposicao entre os eventos agendados
      e o evento que se pretende agendar*/
      for (i=0;i<size;i++)
        if (verify(list[i],list[size],CTRL_N_A))
          control=1;

      /*se o evento for valido, incrementa a dimensao do array */
      if (!control)
          size++;

      return size;

  }

/*funcao que ordena cronologicamente e imprime todos os eventos que ocorrem na sala;
recebe como parametros o array de eventos e a sua dimensao; nao retorna nada*/
void room_sort(Evento list[DIM_MAX], int size){
      int room,i,j=0;
      /*variavel com o valor 1 caso haja, no minimo, um evento a ocorrer na sala*/
      int control=0;
      Evento vec[DIM_MAX];

      scanf("%d",&room);
      /*getchar do \n */
      getchar();

      for(i=0;i<size;i++)
          /*coloca num vetor todos os eventos que ocorrerem na sala*/
          if (list[i].sala==room){
              vec[j++]=list[i];
              control=1;
            }

      if (!control)
          return;

      if (j>1)
          sort(vec,j);
      else
          write(vec[0]);
    }

/*funcao que ordena cronologicamente e imprime todos os eventos agendados;
recebe como argumentos o array de eventos e a sua dimensao; nao retorna nada*/
void events_sort(Evento list[DIM_MAX], int size){
      if (size>1)
          sort(list,size);
      else
          write(list[0]);
    }

/*funcao que copia o evento de indice k+1 para o indice k; recebe como argumentos
o array de evento, o indice a partir do qual se pretente deslocar o array da direita
para a esquerda e a dimensao do array de evenetos;nao retorna nada*/
void change_event(Evento list[DIM_MAX], int k, int size){
      int i,j;

      /*ciclo que percorre todo o array, do indice k ate ao indice size-1,
      substituindo cada evento pelo evento do indice imediatamente a seguir*/
      for (i=k;i<size-1;i++){
          strcpy(list[i].descricao,list[i+1].descricao);
          strcpy(list[i].responsavel,list[i+1].responsavel);
          list[i].data=list[i+1].data;
          list[i].inicio=list[i+1].inicio;
          list[i].duracao=list[i+1].duracao;
          list[i].fim=list[i+1].fim;
          list[i].sala=list[i+1].sala;
          list[i].n_part=list[i+1].n_part;

          for(j=0;j<list[i+1].n_part;j++)
              strcpy(list[i].participantes[j],list[i+1].participantes[j]);
    }
  }

/*funcao que apaga um evento; recebe como argumentos o array de eventos e a sua
dimensao; retorna a dimensao do array de eventos*/
int delete_event(Evento list[DIM_MAX], int size){
      int i;
      /*variavel que apresenta o valor 1 caso algum evento corresponda a descricao
      dada*/
      int control=0;
      char description[MAXSTRING];

      read_input(description);

      for(i=0;i<size;i++)
        if (strcmp(list[i].descricao,description)==0){
            /*ao encontrar o evento que corresponde a descricao, recorre a funcao
            change_event e sobrepoe esse evento*/
            change_event(list,i,size);
            control=1;
            break;
          }

      if (!control){
          printf("Evento %s inexistente.\n", description);
          return size;
        }

      /*se o evento for apagado, decrementa a dimensao do array*/
      size--;
      return size;
    }

/*funcao que altera a hora do evento; recebe como argumentos a lista de eventos
e a sua dimensao; nao retorna nada*/
void change_begin(Evento list[DIM_MAX], int size){

      int i,inicial_old,j=0,end_old;
      /*variavel que apresenta o valor 1 se a descricao de algum evento
      corresponder com a descricao dada*/
      int control=0;
      char input[DIM_INPUT],new[SIZE_EVENTO][MAXSTRING];

      read_input(input);
      /*separa a informacao do input colocando-a no array new, sendo o primeiro
      elemento a descricao e o segundo a hora inicial*/
      separate_string(input,new);

      for (i=0;i<size;i++)
          if (strcmp(list[i].descricao,new[0])==0){
              /*na variavel j guarda o indice do evento que coincide com a descricao dada*/
              j=i;
              inicial_old=list[i].inicio;
              end_old=list[i].fim;
              /*atualiza o evento com a nova hora inicial*/
              list[i].inicio=atoi(new[1]);
              list[i].fim=calculate_end(list[i].inicio,list[i].duracao);
              control=1;
              break;
          }

      if (!control){
          printf("Evento %s inexistente.\n", new[0]);
          return;
      }

      for (i=0;i<size;i++)
            /*garante que o evento nao e comparado com ele mesmo*/
            if ((i!=j) && verify(list[i],list[j],CTRL_N_A)){
                /*se houver alguma sobreposicao com outros eventos a hora inicial
                do evento nao e alterada */
                list[j].inicio=inicial_old;
                list[j].fim=end_old;
            }
    }

/*funcao que altera a duracao do evento; recebe o array de eventos e a sua dimensao;
nao retorna nada*/
void change_duration(Evento list[DIM_MAX], int size){

      int i,j=0,end_old,duration_old;
      /*variavel que apresenta o valor 1 se a descricao de algum evento
      corresponder  com a descricao dada*/
      int control=0;
      char input[DIM_INPUT],new[SIZE_EVENTO][MAXSTRING];


      read_input(input);
      /*separa a informacao do input colocando-a no array new, sendo o primeiro
      elemento a descricao e o segundo a duracao*/
      separate_string(input,new);

      for (i=0;i<size;i++)
          if (strcmp(list[i].descricao,new[0])==0){
              /*na variavel j guarda o indice do evento que coincide com a descricao dada*/
              j=i;
              duration_old=list[i].duracao;
              end_old=list[i].fim;
              /*atualiza o evento com a nova duracao*/
              list[i].duracao=atoi(new[1]);
              list[i].fim=calculate_end(list[i].inicio,list[i].duracao);
              control=1;
              break;
            }

      if (!control){
        printf("Evento %s inexistente.\n", new[0]);
        return;
      }

      for (i=0;i<size;i++)
          /*garante que o evento nao e comparado com ele mesmo*/
          if ((i!=j) && verify(list[i],list[j],CTRL_N_A)){
              /*se houver alguma sobreposicao com outros eventos a duracao
              do evento nao e alterada */
              list[j].duracao=duration_old;
              list[j].fim=end_old;
          }
    }

/*funcao que altera a sala de um evento; recebe o array de eventos e a sua dimensao;
nao retorna nada*/
void change_room(Evento list[DIM_MAX], int size){

      int i,j=0,room_old;
      /*variavel que apresenta o valor 1 se a descricao de algum evento
      corresponder  com a descricao dada*/
      int control=0;
      char input[DIM_INPUT],new[SIZE_EVENTO][MAXSTRING];

      read_input(input);
      /*separa a informacao do input colocando-a novo array new, sendo o primeiro
      elemento a descricao e o segundo a sala*/
      separate_string(input,new);

      for (i=0;i<size;i++)
            if (strcmp(list[i].descricao,new[0])==0){
                /*na variavel j guarda o indice do evento que coincide com a descricao dada*/
                j=i;
                room_old=list[i].sala;
                /*atualiza o evento com a nova sala*/
                list[i].sala=atoi(new[1]);
                control=1;
                break;
            }

        if (!control){
            printf("Evento %s inexistente.\n", new[0]);
            return;
          }

        for (i=0;i<size;i++)
            /*garante que o evento nao e comparado com ele mesmo*/
            if ((i!=j) && verify(list[i],list[j],CTRL_N_A))
            /*se houver alguma sobreposicao com outros eventos a sala do evento
             nao e alterada */
                list[j].sala=room_old;
      }

/*funcao que adiciona um participante a um evento; recebe o array de eventos e sua
dimensao; nao retorna nada*/
void add_part(Evento list[DIM_MAX], int size){

      char input[DIM_INPUT],new[SIZE_EVENTO][MAXSTRING];
      int i,j,k,size_part;
      /*variavel que apresenta o valor 1 se a descricao de algum evento
      corresponder  com a descricao dada*/
      int control=0;

      read_input(input);
      /*separa a informacao do input colocando-a num no array new, sendo o primeiro
      elemento a descricao e o segundo o participnate a adicionar*/
      separate_string(input,new);

      for(i=0;i<size;i++)
        if(strcmp(list[i].descricao,new[0])==0){
            /*na variavel j guarda o indice do evento que coincide com a descricao dada*/
            j=i;
            size_part=list[i].n_part;
            if (size_part==3){
                printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", new[0]);
                return;
              }

            for (k=0;k<size_part;k++){
                /*caso o participante a adicionar ja participe no evento retorna se
                da funcao*/
                if (strcmp(list[i].participantes[k],new[1])==0)
                    return;
              }

            /*adiciona ao array de participantes do evento o novo participante*/
            strcpy(list[i].participantes[size_part],new[1]);
            list[i].n_part++;
            control=1;
            break;
        }

      if (!control){
          printf("Evento %s inexistente.\n",new[0]);
          return;
      }

      for(i=0;i<size;i++)
          /*garante que o evento nao e comparado com ele mesmo*/
          if ((i!=j) && verify(list[i],list[j],CTRL_A)){
              /*se houver alguma sobreposicao ,o participante nao e adicionado,
              sendo substituida a string com o nome do participante por um espaco*/
              strcpy(list[j].participantes[size_part],"");
              list[j].n_part--;
          }
    }

/*funcao que remove um participante; recebe como argumentos o array de eventos e
a sua dimensao; nao retorna nada*/
void remove_part(Evento list[DIM_MAX], int size){

      char input[DIM_INPUT],new[SIZE_EVENTO][MAXSTRING];
      int i,k,size_part,j;
      /*variavel que apresenta o valor 1 se a descricao de algum evento
      corresponder com a descricao dada*/
      int control=0;
      /*variavel que apresenta o valor 1 se o participante a remover participa
      no evento*/
      int control1=0;

      read_input(input);
      /*separa a informacao do input colocando-a num no array new, sendo o primeiro
      elemento a descricao e o segundo o participnate a remover*/
      separate_string(input,new);

      for(i=0;i<size;i++)
        if(strcmp(list[i].descricao,new[0])==0){
          size_part=list[i].n_part;

          for (k=0;k<size_part;k++)
            /*a variavel k contem o indice do participante que se pretende remover*/
            if (strcmp(list[i].participantes[k],new[1])==0){
              control1=1;
              break;
            }

          if (!control1)
              return;

          if (size_part==1){
              printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", new[1], new[0]);
              return;
            }

          for (j=k;j<size_part-1;j++){
              /*remove ao array de participantes do evento o participante*/
              strcpy(list[i].participantes[j],list[i].participantes[j+1]);
              strcpy(list[i].participantes[j+1],"");
          }
          list[i].n_part--;
          control=1;
          break;
        }

      if (!control)
          printf("Evento %s inexistente.\n",new[0]);
}

int main() {
    /*variavel que contem a letra do comando que o utilizador prentende executar*/
    char command;
    /*array com todos os eventos validos agendados*/
    Evento list[DIM_MAX];
    /*numero de eventos agendados*/
    int size=0;

    while(1){
        command=getchar();
        /*getchar do espaco*/
        getchar();

        switch (command) {
          case 'a':
                size=addEvent(list,size);
                break;
          case 'l':
                events_sort(list,size);
                break;
          case 's':
                room_sort(list,size);
                break;
          case 'r':
                size=delete_event(list,size);
                break;
          case 'i':
                change_begin(list,size);
                break;
          case 't':
                change_duration(list,size);
                break;
          case 'm':
                change_room(list,size);
                break;
          case 'A':
                add_part(list,size);
                break;
          case 'R':
                remove_part(list,size);
                break;
          case 'x':
                return 0;
        }
      }

      return 0;
}
