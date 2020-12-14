/*
更新日志&小剧场

把机枪向左移动了五厘米、
修复了一个错误
多出了五个
上面两行是假的

目前完成了前五个功能，后面五个还没做
哦对了，第十个功能是退出，我做完了，现在还差四个
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
    int id;
    char name[12];
    int chi;
    int math;
} Student;

const int BIG=100;
Student StudentList[100];
Student* CurrentOne=&StudentList[0];//这个指针很重要，她会永远指向下一个“等待被输入的槽位”，而每次输出只需要到她的前一位即可
//由于每次删除都会将前后两部分合并，那么每次删除只需要把这个指针向前调一位就好了
//不明白的话就去看del函数的代码！！！

//针对Student结构体编写的复制函数
//其实这个函数根本没有必要，用(*NewStu)=(*Stu)也可以
//但我像个憨憨一样写了这玩意那就接着拿来用吧...
//至少这东西可以拿来改成某种可定制的复制函数，这是唯一比等号优秀的地方...
//为了证明我不是习惯性绕路而是高瞻远瞩我又加了五行注释，真的没这个必要
//别学我，你看我都写了六行废话了
void StuCpy(Student* NewStu,Student* Stu){
    (*NewStu).id=(*Stu).id;
    strcpy((*NewStu).name,(*Stu).name);
    (*NewStu).chi=(*Stu).chi;
    (*NewStu).math=(*Stu).math;
}

void menu(){
    printf("\t\t功能菜单\n");
    printf("<1>添加\t\t\t\t<6>排序\n");
    printf("<2>查看\t\t\t\t<7>登录\n");
    printf("<3>修改\t\t\t\t<8>保存为文件\n"); 
    printf("<4>删除\t\t\t\t<9>从文件加载\n");
    printf("<5>搜索\t\t\t\t<0>退出\n");
}

void eat(){
    //吃掉一个回车
    getchar();
}

void stop(){
    printf("\n<按任意键进行下一步>\n");
    getchar();
}

void apd(){
    int id,chi,math;
    char name[12];
    printf("添加\n\n");
    printf("请输入学号:");
    scanf("%d",&id);
    eat();
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        if(id==(*p).id){
            printf("学号重复，请重试\n");
            return;
        }
    }
    printf("请输入姓名:");
    scanf("%s",name);
    printf("请输入语文成绩:");
    scanf("%d",&chi);
    printf("请输入数学成绩:");
    scanf("%d",&math);
    eat();
    (*CurrentOne).id=id;
    strcpy((*CurrentOne).name,name);
    (*CurrentOne).chi=chi;
    (*CurrentOne).math=math;
    CurrentOne++;
}

//用于单独地输出一个同学的信息
void prt_1(Student* p){
    printf("学号:%d\n",(*p).id);
    printf("姓名:%s\n",(*p).name);
    printf("语文成绩:%d\n",(*p).chi);
    printf("数学成绩:%d\n",(*p).math);
}

//用于一次性输出所有同学的信息
void prt(){
    printf("查看\n\n");
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        prt_1(p);
        if(p!=CurrentOne-1){
            printf("\n");
        }
    }
}

void chg_1(Student* p){
    int choose;
    printf("请选择你想要修改的值\n<1>姓名\t<2>语文\t<3>数学<其他>离开\n");
    scanf("%d",&choose);
    eat();
    if(choose==1){
        char name[12];
        printf("请输入新的姓名:");
        scanf("%s",&name);
        eat();
        strcpy((*p).name,name);
    }
    else if(choose==2){
        int chi;
        printf("请输入新的语文成绩:");
        scanf("%d",&chi);
        eat();
        (*p).chi=chi;
    }
    else if(choose==3){
        int math;
        printf("请输入新的数学成绩:");
        scanf("%d",&math);
        eat();
        (*p).math=math;
    }
    else{
        return;
    }
}

//修改
void chg(){
    printf("修改");
    printf("请输入学号");
    int id;
    scanf("%d",&id);
    eat();
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        if((*p).id==id){
            prt_1(p);
            chg_1(p);
            break;
        }
        if(p==CurrentOne-1){
            printf("没有找到\n");
        }
    }
}

//删除某一项后，该项后面的所有项目都会向前移动一位
void del(){
    printf("删除，在这个过程中被删去的数据，其之后的数据会补位。直到整个表连贯\n");
    Student NewList[BIG];
    Student* np=&NewList[0];
    printf("请输入学号:");
    int id;
    scanf("%d",&id);
    eat();
    //pp的作用是把p的值从循环中带出来
    Student* pp=&StudentList[0];
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        if((*p).id==id){
            pp=p;
            break;
        }
        StuCpy(np,p);
        np++;
        if(p==CurrentOne-1){
            printf("没有找到");
            return;
        }
    }
    for(Student* p=pp+1;p!=CurrentOne;p++){
        StuCpy(np,p);
        np++;
    }
    np=&NewList[0];//把np重置
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        StuCpy(p,np);
        np++;
    }
    CurrentOne--;
}

//查询
void sch(){
    printf("搜索\n\n");
    printf("请输入学号:");
    int id;
    scanf("%d",&id);
    eat();
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        if((*p).id==id){
            prt_1(p);
            break;
        }
        if(p==CurrentOne-1){
            printf("没有找到\n");
        }
    }
}

void rnk(){}
void lgn(){}
void svf(){}
void ldf(){}

//在一个很遥远的时代这个函数里的输出是用来宣泄情绪的...但那真的是很久以前了...
//向Deemo致敬，向永远不会回来的三年时光致敬
//我们永远也回不去了，我们也不会想再回去，但它塑造了如今的我，如同我们和提督的友谊
//向Sylvia和Anastasia纯净、伟大而永恒的友谊致敬
void ext(){
    printf("在离开前不要忘记说最后一次再见\n");
    printf("Never left without saying Goodbye.\n");
}

int main(){
    while(1){
        system("cls");
        menu();
        printf(">>>");
        int choose;
        scanf("%d",&choose);
        eat();
        system("cls");
        switch(choose){
            case 1:apd();break;
            case 2:prt();break;
            case 3:chg();break;
            case 4:del();break;
            case 5:sch();break;
            case 6:rnk();break;
            case 7:lgn();break;
            case 8:svf();break;
            case 9:ldf();break;
            case 0:ext();break;
        }
        stop();
        if(choose==0){
            break;
        }
    }
    return 0;
}
