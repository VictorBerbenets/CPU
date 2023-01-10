#include "stack.h"

void StackCtor(stack* st){

    st -> stack_is_damaged = 0;
    st -> capacity = 0;
    st -> size = 0;
    st -> data = nullptr;
    
    _StackDump(st);
}

void StackDtor(stack* st){ 

    DoPoison(st); 
    
    free(st -> data);

    st -> stack_is_damaged = 0;
    st -> data = nullptr;
    st -> size = 0;
    st -> capacity = 0;

    _StackDump(st);
}

void StackPush(stack* st, Data value){

    _StackCheck(st);

    if (IsStackFull(st) == StackFull || st->capacity == 0){
        StackResize(st, PUSH);
    }

    st -> data[st -> size] = value;
    st -> size ++;

    DoPoison(st);

    _StackCheck(st);
}

Data StackPop (stack* st){

    _StackCheck(st);

    Data res = 0;
    res = st -> data[(st -> size) - 1];

    if ((st->capacity - st->size) > MaxStayedSize){
        StackResize(st, POP);
    }


    if (IsStackEmpty(st) == StackNotEmpty){
        st -> size --;
        DoPoison(st);
        return res;
    }
        return StackIsEmpty;   
}

void StackResize(stack* st, int Push_or_Pop){

    _StackCheck(st);

    switch(Push_or_Pop){

        case PUSH:{ 
            StackRecalloc(Push_or_Pop, st, st->capacity + (st->size / 2) + 1); // увеличиваем capacity, когда нужно место для Push
            break;
        }
        case POP:{ 
            StackRecalloc(Push_or_Pop, st, st->capacity - MinusSize); // уменьшаем capacity, когда достали много элементов из стэка
            break;
        }   
    }
}

void StackRecalloc (int Push_or_POP, stack* st, int new_size){

    _StackCheck(st);

    // size_t old_size = _msize(st->data);

    st -> data = (Data*)realloc(st -> data, sizeof(Data) * new_size);

    //int* tmp_pt; 

    /*if (Push_or_POP == PUSH){
        tmp_pt = st->data + old_size;
        memset (tmp_pt, 0, new_size-(int)old_size);
    }

    else{
        tmp_pt = st->data + new_size;
        memset (tmp_pt, 0, old_size - new_size);
    } 
*/
    st->capacity = new_size;
    
}


void StackCheck(stack* st, int line, const char func[]){ // StackVerificate()


    int err_code = 0;

    if (!st){
        fprintf(stderr, "In line \"%d\", function <%s>: stack = NULL\n", line, func);
        exit(EXIT_FAILURE);
    }

    if (st->size > st->capacity){ 
        err_code += INVALID_CAPACITY;      
    }

    if ((int)st -> size < 0){
        err_code += INVALID_SIZE; 
    }

    if (IsDataValid(st) == ElemIsPoison){
        err_code += ElemOfStackIsPoison;
    }

    if (err_code){
        st -> stack_is_damaged = 1;
    }

    PrintError(err_code, line, func);
}

void PrintError (int ErrCode, int line, const char func[]){

    int number_of_errors = 0;

    FILE* Errors = fopen("Errors.txt", "a");

    if (ErrCode & INVALID_CAPACITY){ 
        fprintf (Errors, "Error in line: %d, Function: <%s>\nSize of the Stack > capacity!\n",line, func);
        number_of_errors ++;
    }
    if (ErrCode & NULL_PTR){
        fprintf(Errors, "Error in Line:  %d, Function: <%s>\nSigmentation fault\n", line, func);
        number_of_errors ++;
    }
    if (ErrCode & INVALID_SIZE){
        fprintf(Errors, "Error in Line:  %d; Function: <%s>\nInvalid size of stack\n", line, func);
        number_of_errors ++;
    }
    if (ErrCode & ElemOfStackIsPoison){
        fprintf(Errors, "Error in Line:  %d; Function: <%s>\nElement of stack is poison\n", line, func);
        number_of_errors ++;
    }

    fclose(Errors);

}

int IsStackFull(stack* st){

    _StackCheck(st);
    
    if (st -> size ==  st -> capacity)
        return StackFull;
    return StackNotFull;
}


int IsStackEmpty(stack* st){

    _StackCheck(st);

    if ((st -> size == 0))
        return StackIsEmpty;
    return StackNotEmpty;
}

void DoPoison(stack*st){


    int difference = st->capacity - st->size;

    for (int i = 0; i < difference; i++){
        st->data[st->size + i] = POISON;
    }

}
int IsDataValid(stack* st){  // IsDataValid

    size_t i;

    for (i = 0; i < st->size; i++){
        if (st->data[i] == POISON){
            return ElemIsPoison;
        }
    }
    return NoPoison;
}

void StackDump (stack* st, int line, const char func[], const char* stack_name)
{          
    /*Вывод общей информации*/
    FILE* log_txt = fopen("log.txt", "a");

    fprintf(log_txt, "\n***********************************************************************************\n");
    fprintf(log_txt, "StackDump was called from '%d' line and function: <%s>\n", line, func);
    fprintf(log_txt, "Stack name: %s\n",                          stack_name);
    fprintf(log_txt, "\t\tElements in stack(st.size):   %zd\n",     st->size);
    fprintf(log_txt, "\t\tStack capacity:               %zd\n", st->capacity); 
    fprintf(log_txt, "\t\tStack pointer(data):          %p\n",      st->data);
    fprintf(log_txt, "\t\tStack is damaged?:           <%s>\n", st -> stack_is_damaged == 1 ? "YES":"NO");
    fprintf(log_txt, "%s\n",st->capacity == 0 ? "Stack is empty":"Elements in stack:");

    for (size_t i = 0; i < st->capacity; i++){

        if (st->data[i] != POISON){

            fprintf(log_txt, "*[%zd] = %lg\n", i, st->data[i]);
        }

        else
            fprintf(log_txt, " [%zd] = %lg POISON\n", i, st->data[i]);
    }

    fprintf(log_txt, "***********************************************************************************\n\n");

    fclose(log_txt);

}

