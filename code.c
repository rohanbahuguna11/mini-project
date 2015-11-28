#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<math.h>
long long int Decimal(char counter[10])
{
    int len = 0, i;
    const int base = 16;
    long long int temp = 0;
    len = strlen(counter);
    for(i = 0; i < len; i++)
    {
        if(counter[i] >= 65 && counter[i] <= 70)
            temp = temp + (((int)(counter[i])) - 55) * pow(base, len - i -1);
        else if(counter[i] >= 48 && counter[i] <= 57)
            temp = temp + (((int)(counter[i])) - 48) * pow(base, len - i -1);
    }
    return temp;
}
int pass2()
{
    FILE *rslt, *ln, *opt, *sm, *rcd;
    char Ad[20], symadd[20], op[20], st[20], lb[20], mne[20], opd[20], sym[20], op2[20], len[20], txt[20];
    int op1[10], i, j = 0, len2;
    rslt = fopen("result.txt","r");
    fscanf(rslt,"%s%s%s%s",Ad,lb,mne,opd);
    ln = fopen("length.txt","r");
    if(strcmp(mne,"START") == 0)
    {
        strcpy(st,opd);
        fscanf(ln,"%s",&len);
    }
    rcd = fopen("record.txt","w");
    printf("H^%s^%s^%s\nT^00%s",lb,opd,len,st);
    fprintf(rcd,"H^%s^%s^%s\nT^00%s",lb,opd,len,st);
    fscanf(rslt,"%s%s%s%s",Ad,lb,mne,opd);
    opt = fopen("optab.txt","r");
    sm = fopen("symtab.txt","r");
    while(strcmp(mne,"END") != 0)
    {
        fscanf(opt,"%s%s",op2,op);
        while(1)
        {
            if(feof(opt))
                break;
            if(strcmp(mne,op2) == 0)
            {
                fscanf(sm,"%s%s",symadd,sym);
                while(!feof(sm))
                {
                    if(strcmp(opd,sym) == 0)
                    {
                        printf("^%s%s",op,symadd);
                        fprintf(rcd,"^%s%s",op,symadd);
                        break;
                    }
                    else
                    {
                        len2 = strlen(opd);
                        if(opd[len2-1] == 'X')
                        {
                            strcpy(txt,opd);
                            txt[len2-2] = '\0';
                            if(strcmp(txt,sym) == 0)
                            {
                                len2 = Decimal(symadd);
                                len2 = len2 + 32768;
                                itoa(len2,symadd,16);
                                printf("^%s%s",op,symadd);
                                fprintf(rcd,"^%s%s",op,symadd);
                                break;
                            }
                        }
                    }
                    fscanf(sm,"%s %s",symadd,sym);
                }
                break;
            }
            else
                fscanf(opt,"%s%s",op2,op);
        }
        if(strcmp(mne,"BYTE") == 0 || strcmp(mne,"WORD") == 0 || strcmp(mne,"RSUB") == 0)
        {
            printf("^");
            if(!strcmp(mne,"WORD"))
            {
                itoa(atoi(opd),opd,16);
                strupr(opd);
                len2 = strlen(opd);
                for(i = 0; i < (6 - len2); i++)
                {
                    printf("0");
                    fprintf(rcd,"0");
                }
                printf("%s",opd);
                fprintf(rcd,"%s",opd);
            }
            else if(strcmp(mne,"RSUB") == 0)
            {
                printf("4C0000");
                fprintf(rcd,"4C0000");
            }
            else
            {
                len2 = strlen(opd);
                if(opd[0] == 'X')
                {
                    for(i = 2; i < (len2 - 1); i++)
                    {
                        printf("%c",opd[i]);
                        fprintf(rcd,"%c",opd[i]);
                    }
                }
                else if((opd[0] == 'C'))
                {
                    len2 = strlen(opd);
                    for(i = 2; i < (len2 - 1); i++)
                    {
                        printf("%X",opd[i]);
                        fprintf(rcd,"%X",opd[i]);
                    }
                }
            }
        }
        fscanf(rslt,"%s%s%s%s",Ad,lb,mne,opd);
        fclose(opt);
        fclose(sm);
        opt = fopen("optab.txt","r");
        sm = fopen("symtab.txt","r");
    }
    if(strcmp(mne,"END") == 0)
    {
        printf("\nE^00%s",st);
        fprintf(rcd,"\nE^00%s",st);
    }
    fclose(rslt);
    fclose(ln);
    fclose(opt);
    fclose(sm);
    fclose(rcd);
    return 0;
}


int pass1()
{
    FILE *inp, *rslt, *sym, *opt;
    char f_nam[30], mnemonics[20], label[50], o1[20], operand[50], address[10], counter[10], o2[10];
    int ln;
    printf("Input file name: ");
    gets(f_nam);
    system("cls");
    inp = fopen(f_nam,"r");
    fscanf(inp,"%s%s%s",label,mnemonics,address);
    if(strcmp(mnemonics,"START") == 0)
    {
        strcpy(counter,address);
        rslt = fopen("result.txt","w");
        fprintf(rslt,"-\t%s\t%s\t%s\n",label,mnemonics,counter);
        fscanf(inp,"%s %s %s",label,mnemonics,operand);
    }
    else
    {
        counter[0] = '0';
        counter[1] = '\0';
    }
    sym = fopen("symtab.txt","w");
    while(1)
    {
        if(feof(inp))
            break;
        fprintf(rslt,"%s\t%s\t%s\t%s\n",counter,label,mnemonics,operand);
        if(strcmp(label,"-") != 0)
            fprintf(sym,"%s\t%s\n",counter,label);
        opt = fopen("optab.txt","r");
        fscanf(opt,"%s%s",o1,o2);
        while(1)
        {
            if(feof(opt))
                break;
            if(strcmp(mnemonics,o1)==0)
            {
                itoa((Decimal(counter)+3),counter,16);
                strupr(counter);
                break;
            }
            fscanf(opt,"%s%s",o1,o2);
        }
        if(strcmp(mnemonics,"WORD")==0)
        {
            itoa((Decimal(counter)+3),counter,16);
            strupr(counter);
        }
        else if(strcmp(mnemonics,"RESW")==0)
        {
            itoa((Decimal(counter)+(3*atoi(operand))),counter,16);
            strupr(counter);
        }
        else if(strcmp(mnemonics,"BYTE")==0)
        {
            if(operand[0]=='X')
            {
                itoa((Decimal(counter)+1),counter,16);
                strupr(counter);
            }
            else
            {
                itoa((Decimal(counter)+(strlen(operand) - 3)),counter,16);
                strupr(counter);
            }
        }
        else if(strcmp(mnemonics,"RESB")==0)
        {
            itoa((Decimal(counter)+atoi(operand)),counter,16);
            strupr(counter);
        }
        fscanf(inp,"%s%s%s",label,mnemonics,operand);
        fclose(opt);
    }
    if(!strcmp(mnemonics,"END"))
    {
        itoa((Decimal(counter) - Decimal(address)),counter,16);
        strupr(counter);
        fclose(inp);
        inp = fopen("Length.txt","w");
        fprintf(inp,"%s",counter);
    }
    fclose(inp);
    fclose(rslt);
    fclose(sym);
    return 0;
}

int main()
{
    pass1();
    pass2();
    return 0;
}
