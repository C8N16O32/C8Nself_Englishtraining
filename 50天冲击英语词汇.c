// 50����Ӣ��ʻ�.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define maxlength 7000
#define FILENAME "math1.txt"
//"vocabulary1.txt"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "string.h"

struct __vocabulary__ {
	int ord;
	char en[32];
	char ch[100];
	char pos1[8];
	char pos2[8];
	int count;
	int lable;
};
struct __set__ {
	int start;
	int end;
	int val_mincount;
	int val_optnummax;
	int val_optnum;
	int val_optnummin;
};
struct __set__ set = { 1,290,2,1,0,1 };
int vlen = 0;

//�������ܣ����ѡ�⣬��¼������ʷ
int set_vlen(struct __vocabulary__ *p) {
	int i = 0;
	while (strlen((p + i)->en) != 0)i++;
	return i;
}
int lable_basic(struct __vocabulary__ *p, int a, int b, int num) {
	int i;
	if (b < a) { printf("�����޳���"); return 1; }
	for (i = a; i <= b; i++)(p + i)->lable = num;
	return 0;
}
int choose1(struct __vocabulary__ *p, int a, int b) {
	int f = lable_basic(p, a - 1, b - 1, 1);
	if (f)return 0;
	return 1;
}
int vmax(struct __vocabulary__ *p) {
	int i, len = vlen, max = 0;
	for (i = 0; i < len; i++)if (max < (p + i)->count)max = (p + i)->count;
	return max;
}
int vmin(struct __vocabulary__ *p) {
	int i, len = vlen, min = 1000;
	for (i = 0; i < len; i++)
		if (min >(p + i)->count && (p + i)->lable>0)min = (p + i)->count;
	return min;
}
int count(struct __vocabulary__ *p, int lable) {
	int i = 0, count = 0;
	while (strlen((p + i)->en) != 0) {
		if ((p + i)->lable == lable)count++;
		i++;
	}
	return count;
}
void choose2(struct __vocabulary__ *p, int min, int max) {
	int i, len = vlen;
	do {
		for (i = 0; i < len; i++)
			if ((p + i)->count >= min && (p + i)->count <= max && (p + i)->lable >= 1)
				(p + i)->lable = 2;
		max++;
	} while (count(p, 2) < 4);
}
void lable(struct __vocabulary__ *p, int i, int num) {
	lable_basic(p, i, i, num);
}
void lable_reset(struct __vocabulary__ *p, int maxlable) {
	int i, len = vlen;
	for (i = 0; i < len; i++)if ((p + i)->lable >= maxlable)(p + i)->lable = maxlable;
}
int vmin2(struct __vocabulary__ *p, int lable) {
	int i, len = vlen, min = 1000;
	for (i = 0; i < len; i++)
		if (min >(p + i)->count && (p + i)->lable == lable)min = (p + i)->count;
	return min;
}
int count2(struct __vocabulary__ *p, int lable, int nowcount) {
	int i = 0, count = 0;
	while (strlen((p + i)->en) != 0) {
		if ((p + i)->lable == lable && (p + i)->count == nowcount)count++;
		i++;
	}
	return count;
}
int randi(struct __vocabulary__ *p, int setlable, int randnum) {
	int i, n = 0, len = vlen, num = count(p, setlable - 1);
	if (num == 0)return -1;
	int nc = vmin2(p, setlable - 1);
	num = count2(p, setlable - 1, nc);
	time_t timep; struct tm *tp; time(&timep); tp = gmtime(&timep);
	srand(tp->tm_sec * tp->tm_min + tp->tm_hour + num + randnum);
	num = rand() % num;
	for (i = 0; i < len; i++) {
		if ((p + i)->lable == setlable - 1 && (p + i)->count == nc)n++;
		if (n - 1 == num)break;
	}
	return i;
}
int rand1(struct __vocabulary__ *p, int setlable) {
	int i = randi(p, setlable, 0);
	lable(p, i, setlable);
	return i;
}
void POS_set(struct __vocabulary__ *p) {
	//��ȡ����
	int n = 0, i = 0, s = 0, e = 0;
	for (n = 0; n < vlen; n++) {
		i = 0; s = 0;
		while (1) {
			e = 0;
			for (i = s; i < 64; i++)
				if ((p + n)->ch[i] == '.')
				{
					e = i;
					break;
				}
			if (e == 0 || s == e + 1)break;

			i = e - 1;
			while ((p + n)->ch[i] <= 'z' && (p + n)->ch[i] >= 'a'&&i>=0)i--;
			if (i == e - 1)break;

			if (strlen((p + n)->pos1) == 0)
				strncpy((p + n)->pos1, (p + n)->ch + i + 1, e - i - 1);
			else if (strlen((p + n)->pos2) == 0)
				strncpy((p + n)->pos2, (p + n)->ch + i + 1, e - i - 1);
			else break;
			s = e + 1;
		}
	}
}
int SPOS(struct __vocabulary__ *p, int j, int k) {
	int i = 0, I = j, len = vlen;
	if (strlen((p + k)->pos1) != 0)
		for (i = 0; i < len; i++) if ((p + (i + j) % len)->lable == 2) {
			if (strcmp((p + (i + j) % len)->pos1, (p + k)->pos1) == 0) { I = (i + j) % len; break; }
			if (strcmp((p + (i + j) % len)->pos2, (p + k)->pos1) == 0) { I = (i + j) % len; break; }
		}
	if (strlen((p + k)->pos2) != 0 && I != j)
		for (i = 0; i < len; i++) if ((p + (i + j) % len)->lable == 2) {
			if (strcmp((p + (i + j) % len)->pos1, (p + k)->pos2) == 0) { I = (i + j) % len; break; }
			if (strcmp((p + (i + j) % len)->pos2, (p + k)->pos2) == 0) { I = (i + j) % len; break; }
		}
	return I;
}
void rand4r(struct __vocabulary__ *p, int setlable, int *k) {
	int i, j, rnum = 0;
	for (i = 0; i < 4; i++) {
		j = randi(p, setlable, rnum);
		if (j == -1)return;

		//��Ҫ����ͬ���������Ѱ����ͬ���ԣ�ֻ�Ų�һ��
		if (*k != -1)j = SPOS(p, j, *k);

		lable(p, j, setlable);
		*(k + i) = j;
		rnum += j * 10 + 130;
		rnum = rnum % 390;
	}
}
void rand4ord(struct __vocabulary__ *p, int setlable, int *k) {
	int i = 0, rnum = 1000, j, f = 1, count = 0;
	while (f&&count<16) {
		count++;
		i = randi(p, setlable, rnum);
		f = 0;
		for (j = i; j < i + 4; j++)
			if ((p + j)->lable != 2)f = 1;
		rnum += i + 13;
		rnum = rnum % 39;
	}
	if (count == 16)return;
	for (j = i; j < i + 4; j++) {
		lable(p, j, setlable);
		*(k + j - i + 4) = j;
	}
	return;
}
void randans(struct __vocabulary__ *p, int *k, int *ans) {
	int i; for (i = 0; i < set.val_optnum; i++)
		*(ans + i) = rand1(p, 4);
}

//�ж�����ĶԴ�չʾ��ȷ��
void show_answer(struct __vocabulary__ *p, int right) {
	if (right)printf("\t��ȷ��%s����%s\n", p->en, p->ch);
	else printf("\tӦΪ��%s����%s\n", p->en, p->ch);
}
void judgeopt(struct __vocabulary__ *p, char *temp, int k[8],int ansorigin[8]) {
	//��������
	int i, j;
	int ans[8] = { 0 }, f = 1;
	if (strcmp(temp, (p + ansorigin[0])->en) == 0)ans[0] = ansorigin[0];
	else for (i = 0; i < set.val_optnum; i++)
	{
		ans[i] = (int)*(temp + i);
		if (ans[i] > 96)ans[i] -= 32;
		ans[i] -= 64 + 1;
		if (ans[i] < 0 || ans[i]>7)f = 0;
	}
	//�ж�
	if (f)
		for (j = 0; j < set.val_optnum; j++)
			if (strcmp((p + *(k + ans[j]))->en, (p + ansorigin[j])->en) == 0
				|| strcmp((p + *(k + ans[j]))->ch, (p + ansorigin[j])->ch) == 0)
			{
				show_answer(p + *(k + ans[j]), 1);
				for (i = 0; i < 8; i++)if (strcmp((p + *(k + i))->en, (p + ansorigin[j])->en) == 0
					|| strcmp((p + *(k + i))->ch, (p + ansorigin[j])->ch) == 0)
					(p + *(k + i))->count += 1;
			}
			else {
				show_answer(p + ansorigin[j], 0);
				for (i = 0; i < 8; i++)if (strcmp((p + *(k + i))->en, (p + ansorigin[j])->en) == 0
					|| strcmp((p + *(k + i))->ch, (p + ansorigin[j])->ch) == 0)
					(p + *(k + i))->count -= 2;
				if (f) {
					show_answer(p + *(k + ans[j]), 0); (p + *(k + ans[j]))->count -= 2;
				}
			}
	return;
}
void judgedictation(struct __vocabulary__ *p, char *temp, int *ans) {
	//��������
	char temp2[8][32] = { "\0" }, *cp = NULL;
	cp = temp; int i, j;
	for (i = 0; i < set.val_optnum; i++) {
		j = 0; while (*(cp + j) != ','&&*(cp + j) != '\0')j++;
		strncpy(temp2[i], cp, j);
		if (*(cp + j) == ',')cp += 1;
		cp += j;
	}
	//Ѱ����ȷ��
	char temp3[8][32] = { "\0" };
	for (i = 0; i < set.val_optnum; i++) {
		strcat(temp3[i], (p + *(ans + i))->en);
	}
	//��һ�Ƚ�
	int f[9] = { 0 }; f[8] = 1;
	for (i = 0; i < set.val_optnum; i++)
		for (j = 0; j < set.val_optnum; j++)
		{
			if (strcmp(temp2[i], temp3[j]) == 0)f[i] = 1;
			else f[8] = 0;
		}
	//���
	if (f[8] == 1 && set.val_optnum>1) {
		printf("\tȫ�ԣ�\n");
		for (i = 0; i < set.val_optnum; i++)(p + *(ans + i))->count += set.val_optnummax;
	}
	else for (i = 0; i < set.val_optnum; i++) {
		show_answer(p + *(ans + i), f[i]);
		if (f[i] == 0)(p + *(ans + i))->count -= 2;
		else (p + *(ans + i))->count += set.val_optnummax;
	}

}

//�ļ���д
void vocabulary_inputfromfile(struct __vocabulary__ *p) {
	FILE *fp = NULL; int i = 0;
	//��ʼ��
	for (i = 0; i < maxlength; i++) {
		memset((p + i)->en, 0, 32);
		memset((p + i)->ch, 0, 100);
	}
	//����
	i = 0;
	fp = fopen(FILENAME, "r");
	while (!feof(fp)) {
		fscanf(fp, "%s\t%[^\n]", (p + i)->en, (p + i)->ch);
		i++;
	}
	fclose(fp);
}
void vocabulary_rewhitetofile(struct __vocabulary__ *p) {
	FILE *fp = NULL; int i = 0;
	//д��
	fp = fopen("vocabulary1.txt", "w");
	for (i = 0; i < vlen; i++)
		fprintf(fp, "%s\t%s\n", (p + i)->en, (p + i)->ch);
	fclose(fp);
}
void option_save(struct __set__ set) {
	FILE *fp = NULL;
	fp = fopen("opition.txt", "w");
	int i = 0, I = sizeof(struct __set__);
	for (i = 0; i < I; i++)
		fprintf(fp, "%d,", *((int*)set.start + i));
	fclose(fp);
}
void option_read(struct __set__ *set) {
	FILE *fp = NULL;
	fp = fopen("option.txt", "r");
	fscanf(fp, "%d,", &(set->start));
	fscanf(fp, "%d,", &(set->end));
	fscanf(fp, "%d,", &(set->val_mincount));
	fscanf(fp, "%d,", &(set->val_optnummax));
	fclose(fp);
}

//�϶����棬��д����
void translation_print1(struct __vocabulary__ *p, int *ans) {
	//��Ŀ
	int i = 0; printf("====д�����������ѡ�");
	for (i = 0; i < set.val_optnum; i++)printf("%d:[%s] \t", i + 1, (p + *(ans + i))->ch);
	printf("\n");
}
void translation_print2(struct __vocabulary__ *p, int *k) {
	//ѡ��
	int i;
	for (i = 0; i < 8; i++) if (*(k + i)>0) {
		printf("%c:%s\t", 64 + i + 1, (p + *(k + i))->en);
		if (i == 3)printf("\n");
	}
	printf("\n");
}
void translation_main(struct __vocabulary__ *p) {
	//��ʼ��
	printf("===/stop:�˳�\t/show:��ʾ����\t��ܣ���ʾѡ����Ŀ\n");
	int i, min, max, ans[8], j, k[8], tn = 27;
	char temp[100];
	for (i = 0; i < vlen; i++) {
		(p + i)->count = 0; (p + i)->lable = 0; (p + i)->ord = i + 1;
	}
	choose1(p, set.start, set.end);
	//��Ҫ����
	while (1) {

		//������ʾ
		min = vmin(p);
		if (min >= set.val_mincount)
			printf("��ϰ����%d����%d �Ѿ�����˵�%d�����ϰ\n", set.start, set.end, min);
		max = vmax(p);
		if (max > min + 2)max = min + 2;
		set.val_optnum = min + 1;
		if (set.val_optnum > set.val_optnummax)set.val_optnum = set.val_optnummax;
		if (set.val_optnum < set.val_optnummin)set.val_optnum = set.val_optnummin;
		//������Ŀ
		for (i = 0; i < 8; i++) { k[i] = -1; ans[i] = -1; }
		choose2(p, min, max);
		rand4r(p, 3, k); rand4ord(p, 3, k);
		//���ɴ�
		randans(p, k, ans);
		//��ӡ��Ŀ
		printf("%d", tn);
		translation_print1(p, ans);
		//��������
		printf("���룺"); scanf("%s", temp);
		//��������
		if (strcmp(temp, "/stop") == 0)break;
		if (strcmp(temp, "/show") == 0) {
			j = 0; for (i = 0; i < vlen; i++)
				if ((p + i)->count < set.val_mincount && (p + i)->lable >= 1)j += set.val_mincount - (p + i)->count;
			printf("---�����Ŀ�껹ʣ%d��\n", j);
			memset(temp, 0, 100);
			continue;
		}
		if (strcmp(temp, "/alter") == 0) {
			i = vlen;
			while (i == vlen) {
				printf("ԭ�ʣ�");
				memset(temp, 0, 32); scanf("%s", temp);
				i = 0; do { if (strcmp((p + i)->en, temp) == 0)break; i++; } while (i < vlen);
				if (i == vlen)printf("��������");
			};
			printf("�µ����壺");
			do { memset(temp, 0, 100); gets(temp); } while (strlen(temp) < 3);
			memset((p + ans[0])->ch, 0, 100);
			strcat((p + ans[0])->ch, temp);
			vocabulary_rewhitetofile(p);
			printf("�Ѹ��´ʿ�\n");
			memset(temp, 0, 100);
			continue;
		}
		if (strcmp(temp, "-") != 0)
		{
			judgedictation(p, temp, ans);
		}
		else {//����ѡ������
			memset(temp, 0, 100);
			translation_print2(p, k);
			printf("ѡ��");
			scanf("%s", temp);
			//�ж�
			if (strcmp(temp, "-") != 0)
				judgeopt(p, temp, k, ans);
			else for (i = 0; i < 8; i++)
				printf("[%s]\n", (p + k[i])->ch);
		}
		//��β
		memset(temp, 0, 100);
		lable_reset(p, 1);
		printf("\n");
		tn++;
	}
}


//������
int main()
{
	struct __vocabulary__ words[maxlength] = { 0 };
	int input;
	//��ȡ���ʱ�
	vocabulary_inputfromfile(words);
	vlen = set_vlen(words);
	POS_set(words);
	//option_read(&set);
	while (1) {
		//��ӡ���˵�
		printf("==vocabulary==\n");
		printf("1.translation\n");
		printf("������������option.txt���\n");
		printf("4.exit\n");
		scanf("%d", &input);
		//����У��
		if (!(input > 0 && input < 5)) {
			printf("�����˴����ֵ");
			input = 0;
			break;
		}
		//ѡ��
		switch (input) {
		case 1:translation_main(words); break;
		//case 2:dictation_main(words); break;
		case 3:;
		default:return 0;
		}
	}
	return 0;
}
