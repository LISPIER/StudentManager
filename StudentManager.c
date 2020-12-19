//更新日志和小剧场已经被永久移除

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BIG 100

typedef struct{
    int id;
    char name[12];
    int chi;
    int math;
} Student;

Student StudentList[BIG];
Student* CurrentOne=&StudentList[0];//这个指针很重要，她会永远指向下一个“等待被输入的槽位”，而每次输出只需要到她的前一位即可
//由于每次删除都会将前后两部分合并，那么每次删除只需要把这个指针向前调一位就好了
//不明白的话就去看del函数的代码！！！

void menu();
void apd();
void prt();void prt_1(Student* p);
void chg();void chg_1(Student* p);
void del();
void sch();
void rnk();
int lgn();
void svf();
void ldf();
void ext();
void eat();
void stop();
int score(Student* p);
void StuCpy();

int main(){
    while(1){
        int lgn_return_value=lgn();
        if(lgn_return_value==0){
            printf("登陆成功");
            stop();
            break;
        }
        else if(lgn_return_value==1){
            printf("用户名错误\n");
        }
        else if(lgn_return_value==2){
            printf("密码错误\n");
        }
    }

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

void menu(){
    printf("\t\t功能菜单\n");
    printf("<1>添加\t\t\t\t<6>排序\n");
    printf("<2>查看\t\t\t\t<7>登录\n");
    printf("<3>修改\t\t\t\t<8>保存为文件\n");
    printf("<4>删除\t\t\t\t<9>从文件加载\n");
    printf("<5>搜索\t\t\t\t<0>退出\n");
}

//添加
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

//用于一次性输出所有同学的信息
void prt(){
    printf("学号\t姓名\t\t语文\t数学\t总分\n");
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        prt_1(p);
    }
}

//用于单独地输出一个同学的信息
//出现了一个新问题，如果一个数据长度大于8，制表符会让整个输出看起来被偏移了
//我的思路就是针对name这一项的输出进行更加精细的定制，以支持制表符
//姓名到下一个数据之间的空间最长是16位，来自两个制表符
//同时我们限定了姓名的最大长度为12位，不会超出，我们只需要在prt_1()上做亿点点调整就行了
//也就是当姓名长度为8位及以上时，采用姓名后仅有一个制表符的输出语句
//姓名长度小于8位时，采用姓名后有两个制表符的输出语句，和标题行保持一致
void prt_1(Student* p){
    if(strlen((*p).name)<8){
        printf("%d\t%s\t\t%d\t%d\t%d\n",(*p).id,(*p).name,(*p).chi,(*p).math,score(p));
    }
    else{
        printf("%d\t%s\t%d\t%d\t%d\n",(*p).id,(*p).name,(*p).chi,(*p).math,score(p));
    }
}

//修改
void chg(){
    printf("修改\n\n");
    printf("请输入学号");
    int id;
    scanf("%d",&id);
    eat();
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        if((*p).id==id){
            printf("学号\t姓名\t\t语文\t数学\t总分\n");
            prt_1(p);
            chg_1(p);
            break;
        }
        if(p==CurrentOne-1){
            printf("没有找到\n");
        }
    }
}

//修改一个学生的信息
void chg_1(Student* p){
    int choose;
    printf("请选择你想要修改的值\n<1>姓名\t<2>语文\t<3>数学<其他>离开\n");
    scanf("%d",&choose);
    eat();
    if(choose==1){
        char name[12];
        printf("请输入新的姓名:");
        scanf("%s",name);
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

//删除某一项后，该项后面的所有项目都会向前移动一位
void del(){
    printf("删除，在这个过程中被删去的数据，其之后的数据会补位。直到整个表连贯\n\n");
    //新表的作用是将被删去的数据的前后的所有数据逐一放入
    //然后再用新表覆盖旧表
    Student NewList[BIG];
    Student* np=&NewList[0];
    printf("请输入学号:");
    int id;
    scanf("%d",&id);
    eat();
    //我是真的不喜欢把循环中使用的指针或数字定义成循环外可以访问的变量，所以在此处只能用pp把p的值从循环中带出来了
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
    CurrentOne--;//因为删去了一个数据，整个列表向前缩水了一位，所以这个指向第一个空位的指针也要向前拨动一位
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

//排序
//原理很简单，对原来的表格做一次冒泡排序就行了
void rnk(){
    //创建一个备份表格用来暂时存储没有排序的原表格
    Student StudentList_tmp[BIG];
    Student* p_tmp=&StudentList_tmp[0];
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        (*p_tmp)=(*p);
        p_tmp++;
    }
    //以学生的总成绩为指标进行冒泡排序
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        for(Student* pp=&StudentList[0];pp!=CurrentOne-1;pp++){
            if(score(pp)<score(pp+1)){
                Student tmp=(*pp);
                (*pp)=(*(pp+1));
                (*(pp+1))=tmp;
            }
        }
    }
    prt();
    //将备份的表格写回源表格
    p_tmp=&StudentList_tmp[0];
    for(Student* p=&StudentList[0];p!=CurrentOne;p++){
        (*p)=(*p_tmp);
        p_tmp++;
    }
}

//登录功能
//用户名错误返回1
//密码错误返回2
//登陆成功返回0
int lgn(){
    char name[12]="sxy";
    char name_input[12];
    char password[20]="205501020326";
    char password_input[20];
    printf("请输入用户名:");
    scanf("%s",name_input);
    eat();
    if(strcmp(name,name_input)!=0){
        return 1;
    }
    printf("请输入密码:");
    scanf("%s",password_input);
    eat();
    if(strcmp(password,password_input)!=0){
        return 2;
    }
    return 0;
}

void svf(){}
void ldf(){}

//向DEEMO致敬
void ext(){
    printf("在离开前不要忘记说最后一次再见\n");
    printf("Never left without saying Goodbye.\n");
}

void eat(){
    //吃掉一个回车
    getchar();
}

void stop(){
    printf("\n<按任意键进行下一步>\n");
    getchar();
}

int score(Student* p){
    return (*p).chi+(*p).math;
}

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
