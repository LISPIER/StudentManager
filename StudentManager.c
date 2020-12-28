#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BIG 100

typedef struct{
    char id[17];
    char name[17];//精心设计的长度
    int chi;
    int math;
} Student;

Student StudentList[BIG];
int NextPos=0;

//一些结构
int menu();

//主要的功能函数
void new();
void view(); void ViewSingle(int pos,int ViewTitle);
void change();
void delete();
void search();
void rank();
void read();
void save();
void login();
void logout();

//工具
int score(int pos);
int find(char* id);

//一些零碎的函数
void eat();

int main(){
    //登录
    login();

    //菜单循环
    while(1){
        int menu_return=menu();
        system("cls");
        switch(menu_return){
            case 1:new();break;
            case 2:view();break;
            case 3:change();break;
            case 4:delete();break;
            case 5:search();break;
            case 6:rank();break;
            case 7:read();break;
            case 8:save();break;
            case 9:login();break;
            case 0:logout();break;
        }
        system("pause");
        if(menu_return==0){
            break;
        }
    }
    return 0;
}

//显示功能菜单，返回用户所选择的值
int menu(){
    system("cls");
    printf("\t功能菜单\n");
    printf("<1>新建记录\t<6>排序显示\n");
    printf("<2>显示全部\t<7>从文件读取\n");
    printf("<3>修改记录\t<8>保存至文件\n");
    printf("<4>删除记录\t<9>登录界面\n");
    printf("<5>搜索记录\t<0>退出系统\n");
    printf(">>>");
    int choose;
    scanf("%d",&choose);eat();
    return choose;
}

void new(){
    char id[BIG],name[BIG];
    int chi,math;
    printf("当前操作:向学生列表的末尾添加一条新的学生记录\n\n");
    while(1){
        printf("请输入学号(最多16位):");
        scanf("%s",id);eat();
        if(strlen(id)>16){
            printf("输入过长\n");
            continue;
        }
        //检查学号是否有重复
        int SameAsBefore=0;//用于表示是否找到了一个相同的学号，找到了则为1，没找到则默认为0
        for(int i=0;i<NextPos;i++){
            if(strcmp(StudentList[i].id,id)==0){
                SameAsBefore=1;
                break;
            }
        }
        //如果重复则禁止继续操作并接着输入学号
        if(SameAsBefore==1){
            printf("学号重复，请重试!\n");
            continue;
        }
        //经过了n重检查后终于可以把输入的学号填进去了
        strcpy(StudentList[NextPos].id,id);
        printf("%s\n",StudentList[NextPos].id);
        break;
    }
    while(1){
        printf("请输入姓名(最多16位，每个汉字占2位):");
        scanf("%s",name);eat();
        if(strlen(name)>16){
            printf("输入过长\n");
            continue;
        }
        strcpy(StudentList[NextPos].name,name);
        printf("%s\n",StudentList[NextPos].name);
        break;
    }
    printf("请输入语文成绩:");
    scanf("%d",&chi);eat();
    StudentList[NextPos].chi=chi;
    printf("请输入数学成绩");
    scanf("%d",&math);eat();
    StudentList[NextPos].math=math;
    NextPos++;
}

void view(){
    printf("学号\t\t姓名\t\t语文\t\t数学\t\t总成绩\n");
    for(int i=0;i<NextPos;i++){
        ViewSingle(i,0);
    }
}

//打印指定下标的学生信息
//如果ViewTitle为1，则会在打印这个学生的信息时加上表头
void ViewSingle(int pos,int ViewTitle){
    if(ViewTitle==1){
        printf("学号\t\t姓名\t\t语文\t\t数学\t\t总成绩\n");
    }
    //打印学号
    printf("%s",StudentList[pos].id);
    if(strlen(StudentList[pos].id)<8){
        printf("\t\t");
    }
    else if(strlen(StudentList[pos].id)!=16){
        printf("\t");
    }
    //打印姓名
    printf("%s",StudentList[pos].name);
    if(strlen(StudentList[pos].name)<8){
        printf("\t\t");
    }
    else if(strlen(StudentList[pos].name)!=16){
        printf("\t");
    }
    //打印剩下的三个成绩
    printf("%d\t\t%d\t\t%d\n",StudentList[pos].chi,StudentList[pos].math,score(pos));
}

void change(){
    printf("当前操作:通过学号查找学生信息，并修改\n\n");
    char id[17];
    int pos;
    while(1){
        printf("请输入一个有效的学号:");
        scanf("%s",id);eat();
        if(find(id)!=-1){
            break;
        }
    }
    pos=find(id);
    ViewSingle(pos,1);
    printf("请选择你想要变更的值\n");
    printf("<1>姓名<2>语文<3>数学<其他>离开\n");
    printf(">>>");
    int choose;
    scanf("%d",&choose);eat();
    if(choose==1){
        //修改姓名
        printf("请输入修改后的姓名:");
        scanf("%s",StudentList[pos].name);eat();
    }
    else if(choose==2){
        //修改语文
        printf("请输入修改后的语文成绩:");
        scanf("%d",&StudentList[pos].chi);eat();
    }
    else if(choose==3){
        //修改数学
        printf("请输入修改后的数学成绩:");
        scanf("%d",&StudentList[pos].math);eat();
    }
    else{
        return;
    }
    printf("是否要继续修改?(Y/N))");
    char YorN;
    scanf("%c",&YorN);eat();
    if(YorN=='Y'||YorN=='y'){
        system("cls");
        change();
    }
}

//删除的原理就是让被删除的项目之后的每一项向前移一位，然后就可以直接把被删除的项目覆盖掉
//记得移动那个指针
void delete(){
    printf("当前操作，通过学号删除一个学生的信息\n");
    printf("请输入一个有效的学号:");
    char id[17];
    scanf("%s",id);eat();
    if(find(id)!=-1){
        ViewSingle(find(id),1);
        printf("看上去我们找到了你想要删除的数据，你确认要删除吗？(Y/N)");
        char YorN;
        scanf("%c",&YorN);eat();
        if(YorN=='Y'||YorN=='y'){
            for(int i=find(id);i<NextPos;i++){
                StudentList[i]=StudentList[i+1];
            }
            NextPos--;
        }
        printf("请问是否要继续删除？(Y/N)");
        scanf("%c",&YorN);eat();
        if(YorN=='Y'||YorN=='y'){
            system("cls");
            delete();
        }
    }
}

void search(){
    printf("当前操作:通过学号查找学生信息\n\n");
    printf("请输入一个有效的学号:");
    char id[17];
    scanf("%s",id);eat();
    if(find(id)!=-1){
        ViewSingle(find(id),1);
    }
}

//排序
void rank(){
    Student StudentList_tmp[BIG];
    for(int i=0;i<NextPos;i++){
        StudentList_tmp[i]=StudentList[i]; //备份原表
    }
    for(int i=0;i<NextPos;i++){
        for(int j=0;j<NextPos;j++){
            if(score(j)<score(j+1)){
                Student Student_tmp=StudentList[j];
                StudentList[j]=StudentList[j+1];
                StudentList[j+1]=Student_tmp;
            }
        }
    }
    view();
    for(int i=0;i<NextPos;i++){
        StudentList[i]=StudentList_tmp[i]; //将备份写回原表
    }
}

//从StudentData.txt读取
void read(){
    FILE *p=fopen("StudentData.txt","rb");

    //声明临时存储变量
    char id[17],name[17];
    int chi,math;

    for(int i=0;i<NextPos;i++){
        fscanf(p,"%s %s %d %d \n",id,name,&chi,&math);

        strcpy(StudentList[i].id,id);
        strcpy(StudentList[i].name,name);
        StudentList[i].chi=chi;
        StudentList[i].math=math;
    }
    fclose(p);
}

//存储为StudentData.txt
void save(){
    FILE *p=fopen("StudentData.txt","wb");

    //声明临时存储变量
    char id[17],name[17];
    int chi,math;

    for(int i=0;i<NextPos;i++){
        strcpy(id,StudentList[i].id);
        strcpy(name,StudentList[i].name);
        chi=StudentList[i].chi;
        math=StudentList[i].math;

        fprintf(p,"%s %s %d %d \n",id,name,chi,math);
    }
    fclose(p);
}

void login(){
    char username[16]="sxy";
    char username_input[16];
    char password[16]="26";
    char password_input[16];

    while(1){
        printf("输入用户名:");
        scanf("%s",username_input);eat();
        if(strcmp(username,username_input)==0){
            break;
        }
        printf("用户名错误!\n");
    }

    while(1){
        printf("输入密码:");
        scanf("%s",password_input);eat();
        if(strcmp(password,password_input)==0){
            break;
        }
        printf("密码错误!\n");
    }
}

void logout(){
    printf("在离开前，别忘记说最后一次再见。\n");
    printf("Never left without saying Goodbye.\n");
}

//输入一个学生的下标，返回这个学生的成绩
int score(int pos){
    return StudentList[pos].chi+StudentList[pos].math;
}

//给出学号，尝试通过学号查找一个对象
//查找到则返回下标
//没找到则返回-1
int find(char* id){
    int pos;
    for(pos=0;pos<BIG;pos++){
        if(strcmp(StudentList[pos].id,id)==0){
            return pos;
        }
    }
    printf("没有找到\n");
    return -1;
}

void eat(){
    getchar();
}
