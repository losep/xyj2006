// vote command..
// Rewritten by Sunyc@Kync xajh.7758.com 5555 2001��8��30��
// All Copyrights.

#include <ansi.h>
#include <dbase.h>
#include <mudlib.h>

inherit F_SAVE;
inherit F_CLEAN_UP;

/*ȡ�ñ���*/
int get_subject(string arg);
/*ȡ��ѡ����Ŀ*/
int get_choice_number(string arg);
/*ȡ��ѡ������*/
int get_choice(string return_choice);
/*����*/
int help(object me);
/*ͶƱ*/
int do_vote(int i,string arg);
/*ɾ������*/
int do_del_vote(int i);
/*�����ǳ��ñ���*/
int number,number1;
string subject,return_choice;
mapping choice=([]);
object ob=this_object();
string *letters=({"!","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"});
/*���ݰ�*/
object data=load_object("/d/wiz/obj/vote_ban");

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string choice;
    int i,z,j,num;
    if (!arg) { help(me); return 1;}
    if (sscanf(arg,"%d %s",i,choice)==2)
    {
	if (((int)me->query("combat_exp")<300000) && !wizardp(me))
	    return notify_fail("����������������붯��ͶƱ��\n");
	if(wizardp(me)) return notify_fail("��ʦ��ֹ����ͶƱ��\n");
	if(i<=0) return notify_fail("���⵷����Ҫ�ܳͷ��ġ�\n");
	if (!data->query_data()) return notify_fail("����û��һ�����飬ͶʲôƱ����\n");
	if (i>sizeof(data->query_data())) return notify_fail("����������\n");
	if (!data->query_data()[i-1]["choice"][choice]) return notify_fail("�����ѡ���\n");
	if (member_array(me->query("id"),data->query_choice()[i-1]["id"])!=-1)
	    return notify_fail("���Ѿ�Ͷ��Ʊ�ˡ�\n");
	do_vote(i,choice);
	return 1;
    }
    else    if(sscanf(arg,"%d",num)==1)
    {
	if(!data->query_data()) return notify_fail("Ŀǰû���κζ��顣\n");
	if(num>sizeof(data->query_data())) return notify_fail("û�������š�\n");
	data->do_list(num-1);
	return 1;
    }
    else    if(arg=="list") {
	if(!data->query_data()) return notify_fail("Ŀǰû���κζ��顣\n");
	data->do_list(-1);
	return 1;
    }
    else    if(arg=="add" && wizardp(me)) {
	if(sizeof(data->query_data())>5) return notify_fail("Ŀǰ������Ŀ���࣬���ǵȵȰɡ�\n");
	write("���趨һ�����⣺");
	input_to("get_subject");
	return 1;
    }
    else   if(sscanf(arg,"del %d",num)==1)
    {
	if((mapping)data->query_data()==([])) return notify_fail("Ŀǰû���κζ��顣\n");
	if(num>sizeof(data->query_data())) return notify_fail("û�������š�\n");
	data->del(num-1);
	return 1;
    }
    help(me);
    return 1;
}

int get_subject(string arg)
{
    if(arg!="") 
    {
	subject=arg;
	write("���趨ѡ�������");
	input_to("get_choice_number",0);
	return 1;
    }
    write("���趨һ����Ŀ��");
    input_to("get_subject");
    return 1;
}
void choice(int i)
{
    object me=this_player();
    i=number1;
    if(i) 
    {
	choice+=([letters[i]:return_choice]);
    }
    i=i+1;
    number1=i;	
    if(i>number) 
    {	
	data->add_vote(me->short(1),ctime(time()),subject,choice);
	message("channel:chat",HIG+"��������"+this_player()->name(1)+"��"+MUD_NAME+"�ύ��һ�ݶ��顣\n"NOR,users());
	number1=0;
	number=0;
	return;
    }
    write("���������"+i+"��ѡ��Ľ��ܣ�");
    input_to("get_choice");
    return;
}
int get_choice_number(string arg)
{
    if(!arg || sscanf(arg,"%d",number)!=1)
    {    
	write("�����趨ѡ��ĸ�����");
	input_to("get_choice_number");
	return 1;
    }
    choice(0);
    return 1;
}
int get_choice(string arg)
{
    if(arg=="")
    {
	write("������������ѡ����ܣ�");
	input_to("get_choice");
	return 1;
    }
    return_choice=arg;
    choice(number1);
    return 1;
}
int do_vote(int j,string arg)
{
    object me=this_player();
    message("channel:chat",HIG+"��������"+this_player()->name(1)+"��"+MUD_NAME+"��"+j+"�Ŷ����ύ��һƱ��\n"NOR,users());
    data->add_vote_piao(j,arg,me->query("id"));
    return 1;
}
int help(object me)
{
    write(@HELP
ָ���ʽ : 
���µ�vote1ϵͳ֧���������
��ң�
vote1 list   			    �쿴��ǰ�Ķ����б�
vote1 <������> <ѡ��>              ��һ������ͶƱ
vote1 <������>                    �쿴����ľ������

��ʦ��
vote1 add   			    ����һ������
vote1 del <������>                 ɾ��һ������
HELP);
    return 1;
}
