/**
 *
 *  Mini File System Project
 *
 *		Copyright (c) 2018 Linfeng Li. All rights reserved.
 *
**/
#pragma warning (disable:4996)
#include "../include/mfs_coresystem.h"
#include "../include/mfs_algorithm.h"


/// <summary> ��ʾ��ǰ�ļ�Ŀ¼ </summary>
int MiniFileSystem::showDirectory(void)
{
	Directory current_directory = directory[directory.size() - 1];
	struct tm* time;
	uint_32 dir_num = 0, file_num = 0;

	// �����ǰ·��
	putchar('\n');
	printf(" Mini-FS:");
	std::vector<Directory>::iterator iter;
	for (iter = directory.begin(); iter != directory.end(); iter++)
		printf("\\%s", (*iter).header.name);
	printf(" ��Ŀ¼\n\n");

	// ��ǰ���Ǹ�Ŀ¼�����.��..Ŀ¼
	if (directory.size() > 1) {
		Directory &parent_directory = directory[directory.size() - 2];
		dir_num += 2;
		time = gmtime(&current_directory.header.modify_time);
        if ((8 + time->tm_hour) % 24 != 8 + time->tm_hour)
            time->tm_mday++;
		printf("%04d/%02d/%02d  %02d:%02d", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, (8 + time->tm_hour)%24, time->tm_min);
		printf("    <DIR>          .\n");
		time = gmtime(&parent_directory.header.modify_time);
        if ((8 + time->tm_hour) % 24 != 8 + time->tm_hour)
            time->tm_mday++;
		printf("%04d/%02d/%02d  %02d:%02d", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, (8 + time->tm_hour)%24, time->tm_min);
		printf("    <DIR>          ..\n");
	}

	// �������Ŀ¼���ļ�
	for (uint_32 i = 0; i < current_directory.header.file_num; i++){
		if (current_directory.fcb[i].delete_flag == 1)
			continue;
		time = gmtime(&current_directory.fcb[i].modify_time);
        if ((8 + time->tm_hour) % 24 != 8 + time->tm_hour)
            time->tm_mday++;
		printf("%04d/%02d/%02d  %02d:%02d", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, (8 + time->tm_hour)%24, time->tm_min);
		if (current_directory.fcb[i].mark == 1) {
			printf("%18lld ", current_directory.fcb[i].file_size);
			file_num++;
		}
		else if (current_directory.fcb[i].mark == 0) {
			printf("    <DIR>          ");
			dir_num++;
		}
		printf("%s\n", current_directory.fcb[i].name);
	}
	printf("            %d ���ļ�      %d ��Ŀ¼\n", file_num, dir_num);

	return 1;
}


/// <summary> ���δ�ӡ��ǰ�ļ������ļ�Ŀ¼ </summary>
void MiniFileSystem::treeDirectory(void)
{
	uint_32 cur_dir_entrance = directory[directory.size() - 1].header.current_dir_entrance;
	std::vector<int> path;

	// ��ǰĿ¼�����ļ�
	puts("");
	if (directory[directory.size() - 1].header.file_num == 0){
		puts(" û�����ļ���");
		return;
	}

	// �ݹ������Ŀ¼
	treeRecur(cur_dir_entrance, path);
	path.clear();
}


/// <summary> ����ļ�ռ�ôغ� </summary>
/// <return> -1:�����ļ�ʧ��; 1:����ɹ� </return>
int MiniFileSystem::occupyCluster(const char filename[]){
	Directory current_directory = directory[directory.size() - 1];

	// ���������ļ�
	uint_32 file_num = current_directory.header.file_num;
	for (uint_32 i = 0; i < file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].delete_flag == 0) {
			uint_32 cur_cluster = current_directory.fcb[i].data_entrance;
			int count_cluster = 0;
			printf("\n %s ռ�õ�ȫ�����Ϊ:\n", filename);

			// ����FAT�����ռ�ÿ�
			while (cur_cluster != ECOF){
				count_cluster++;
				if (count_cluster % 20 == 0)
					puts("");
				printf(" %d%c", cur_cluster, FAT[cur_cluster] == ECOF ? '\n' : ',');
				cur_cluster = FAT[cur_cluster];
			}
			return 1;
		}
	}

	return -1;
}


/// <summary> �ı䵱ǰ�����ļ���λ�� </summary>
/// <return> -1:�ı�ʧ��; 1:�ı�ɹ� </return>
int MiniFileSystem::changeDirectory(const char filename[])
{
	Directory current_directory = directory[directory.size() - 1];

	// �����ϼ�Ŀ¼
	if (strcmp(filename, "..") == 0) {
		if (directory.size() <= 1)
			return 1;
		free(current_directory.fcb);
		directory.erase(directory.end() - 1);
		return 1;
	}
	// ���뵱ǰĿ¼
	else if (strcmp(filename, ".") == 0) {
		return 1;
	}

	// ����Ŀ¼Ѱ���¼�Ŀ¼
	uint_32 file_num = current_directory.header.file_num;
	for (uint_32 i = 0; i < file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].delete_flag == 0) {
			if (current_directory.fcb[i].mark == 1)
				return -1;
			else {
				Directory next_directory = readDirectory(current_directory.fcb[i].data_entrance);
				directory.push_back(next_directory);
				return 1;
			}
		}
	}
	return -1;
}


/// <summary> �����ļ��� </summary>
int MiniFileSystem::makeDir(const char filename[])
{
	Directory &current_directory = directory[directory.size() - 1];

	// �����Ƿ���������ļ���
	for (uint_32 i = 0; i < current_directory.header.file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].mark == 0) {
			return -1;
		}
	}

	// ����Ŀ¼�ļ�ͷ
	current_directory.header.modify_time = time(NULL);
	current_directory.header.file_num++;
	current_directory.header.occupy_cluster_num = current_directory.header.file_num / 1024 + 1;

	// ����Ŀ¼��Ϣ
	uint_32 file_num = current_directory.header.file_num;
	current_directory.fcb = (FCB *)realloc(current_directory.fcb, file_num * sizeof(FCB));
	strcpy(current_directory.fcb[file_num - 1].name, filename);
	current_directory.fcb[file_num - 1].mark = 0;
	current_directory.fcb[file_num - 1].attribute = ATT_ARCHIVE;
	current_directory.fcb[file_num - 1].delete_flag = 0;
	current_directory.fcb[file_num - 1].occupy_cluster_num = 1;
	current_directory.fcb[file_num - 1].file_size = 0;
	current_directory.fcb[file_num - 1].create_time = current_directory.header.modify_time;
	current_directory.fcb[file_num - 1].modify_time = current_directory.header.modify_time;
	uint_32 empty_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
	MfsAlg::BitSet(CAB, mbr.cluster_num, empty_room);
	FAT[empty_room] = ECOF;
	mbr.free_cluster_num -= 1;
	current_directory.fcb[file_num - 1].data_entrance = empty_room;

	// �ɵ�ǰĿ¼����Ŀ¼���ϸ���
	std::vector<Directory>::reverse_iterator iter;
	uint_32 last_cluster = current_directory.header.current_dir_entrance;
	for (iter = directory.rbegin(); iter != directory.rend(); iter++) {
		Directory &cur_dir = (*iter);
		cur_dir.header.modify_time = current_directory.header.modify_time;
		uint_32 file_num = cur_dir.header.file_num;
		for (uint_32 i = 0; i < file_num; i++) {
			if (cur_dir.fcb[i].data_entrance == last_cluster) {
				cur_dir.fcb[i].modify_time = current_directory.header.modify_time;
				break;
			}
		}
		last_cluster = cur_dir.header.current_dir_entrance;
		rewriteDirectory(cur_dir);
	}

	// �������ļ�ͷ
	Directory dir_directory;
	strcpy(dir_directory.header.name, filename);
	dir_directory.header.occupy_cluster_num = 1;
	dir_directory.header.current_dir_entrance = empty_room;
	dir_directory.header.parent_dir_entrance = current_directory.header.current_dir_entrance;
	dir_directory.header.file_num = 0;
	dir_directory.header.create_time = current_directory.header.modify_time;
	dir_directory.header.modify_time = current_directory.header.modify_time;
	dir_directory.header.folder_size = 0;
	dir_directory.fcb = (FCB *)calloc(1, sizeof(FCB));
	newWriteDirectory(dir_directory);
	free(dir_directory.fcb);

	return 1;
}


/// <summary> �����ļ� </summary>
/// <return> -1:����ʧ��; 1:�����ɹ� </return>
int MiniFileSystem::createFile(const char filename[])
{
	Directory &current_directory = directory[directory.size() - 1];

	// ���������Ƿ��������ļ�
	for (uint_32 i = 0; i < current_directory.header.file_num; i++) {
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].mark == 1 && current_directory.fcb[i].delete_flag == 0) {
			return -1;
		}
	}

	// ����Ŀ¼�ļ�ͷ
	current_directory.header.modify_time = time(NULL);
	current_directory.header.file_num++;
	current_directory.header.occupy_cluster_num = current_directory.header.file_num / 1024 + 1;

	// ����Ŀ¼��Ϣ
	uint_32 file_num = current_directory.header.file_num;
	current_directory.fcb = (FCB *)realloc(current_directory.fcb, file_num * sizeof(FCB));
	strcpy(current_directory.fcb[file_num - 1].name, filename);
	current_directory.fcb[file_num - 1].mark = 1;
	current_directory.fcb[file_num - 1].attribute = ATT_ARCHIVE;
	current_directory.fcb[file_num - 1].delete_flag = 0;
	current_directory.fcb[file_num - 1].occupy_cluster_num = 1;
	current_directory.fcb[file_num - 1].file_size = 0;
	current_directory.fcb[file_num - 1].create_time = current_directory.header.modify_time;
	current_directory.fcb[file_num - 1].modify_time = current_directory.header.modify_time;
	uint_32 empty_room = MfsAlg::BitFindRoom(CAB, mbr.cluster_num);
	MfsAlg::BitSet(CAB, mbr.cluster_num, empty_room);
	FAT[empty_room] = ECOF;
	mbr.free_cluster_num -= 1;
	current_directory.fcb[file_num - 1].data_entrance = empty_room;

	// �ɵ�ǰĿ¼����Ŀ¼���ϸ���
	std::vector<Directory>::reverse_iterator iter;
	uint_32 last_cluster = current_directory.header.current_dir_entrance;
	for (iter = directory.rbegin(); iter != directory.rend(); iter++) {
		Directory &cur_dir = (*iter);
		cur_dir.header.modify_time = current_directory.header.modify_time;
		uint_32 file_num = cur_dir.header.file_num;
		for (uint_32 i = 0; i < file_num; i++) {
			if (cur_dir.fcb[i].data_entrance == last_cluster) {
				cur_dir.fcb[i].modify_time = current_directory.header.modify_time;
				break;
			}
		}
		last_cluster = cur_dir.header.current_dir_entrance;
		rewriteDirectory(cur_dir);
	}

	return 1;
}


/// <summary> ����ƥ������ļ� </summary>
int MiniFileSystem::findFile(const char expression[]){
	Directory cur_directory = directory[directory.size() - 1];
	std::vector<std::string> path;

	// ��¼��ǰ·��
	for (int i = 0; i < (int)directory.size() - 1; i++)
		path.push_back(directory[i].header.name);

	// �ݹ�ƥ����Ŀ¼
	bool empty = true;
	puts("");
	findRecur(expression, cur_directory.header.current_dir_entrance, path, empty);
	if (empty == true)
		puts(" ��ƥ���ļ�");

	return 1;
}


/// <summary> �ƶ��ļ� </summary>
/// <return> -1:��ǰ�ڱ��ƶ��ļ���; -2:�ƶ�ʧ��; 1:�ƶ��ɹ� </return>
int MiniFileSystem::moveFile(const char filename_1[], const char filename_2[]){
	Directory *current_directory = &readDirectory(mbr.RDF_entrance);
	std::vector<std::string> path;

	// �ָ�·��
	MfsAlg::cutPath(filename_1, path);
	if (path[0] == "")
		path.erase(path.begin());

	// Ѱ��Ŀ���ļ�
	for (int i = 1; i < (int)path.size() - 1; i++) {
		uint_32 file_num = current_directory->header.file_num;
		for (uint_32 j = 0; j < current_directory->header.file_num; j++) {
			if (current_directory->fcb[j].name == path[i] && current_directory->fcb[j].delete_flag == 0 && current_directory->fcb[j].mark == 0) {
				current_directory = &readDirectory(current_directory->fcb[j].data_entrance);
				break;
			}
		}
	}

	// �жϵ�ǰĿ¼�Ƿ��ڱ��ƶ�Ŀ¼��
	for (uint_32 i = 0; i < directory.size() - 1; i++){
		if (current_directory->header.current_dir_entrance == directory[i].header.current_dir_entrance){
			return -1;
		}
	}
	if (current_directory->header.current_dir_entrance == directory[directory.size() - 1].header.current_dir_entrance)
		current_directory = &directory[directory.size() - 1];

	// ·���Ϸ����ƣ��Ƿ��򱨴�
	if (copyFile(filename_1, filename_2) == -1)
		return -2;

	// ɾ�����ƶ�·��
	for (uint_32 i = 0; i < current_directory->header.file_num; i++){
		if (current_directory->fcb[i].name == path[path.size() - 1] && current_directory->fcb[i].delete_flag == 0) {
			current_directory->header.folder_size -= current_directory->fcb[i].file_size;
			if (current_directory->fcb[i].mark == 0)
				delRecur(current_directory->fcb[i].data_entrance);
			else{
				uint_32 cur_cluster = current_directory->fcb[i].data_entrance;
				while (cur_cluster != ECOF){
					MfsAlg::BitReset(CAB, mbr.cluster_num, cur_cluster);
					cur_cluster = FAT[cur_cluster];
				}
			}
			current_directory->fcb[i] = current_directory->fcb[current_directory->header.file_num - 1];
			current_directory->header.file_num -= 1;
			rewriteDirectory(*current_directory);
			break;
		}
	}

	return 1;
}


/// <summary> ��ʾ�ļ����� </summary>
/// <return> -1:�����ļ�ʧ��; 1:��ʾ�ɹ� </return>
int MiniFileSystem::showAttribute(const char filename[])
{
	Directory current_directory = directory[directory.size() - 1];
	struct tm* time;

	puts("");

	// ��ʾ��ǰĿ¼�������ļ�����
	if (strcmp(filename, ".") == 0) {
		printf(" %s �ļ����Ѵ�� %lld �ֽ�����\n",
			current_directory.header.name,
			current_directory.header.folder_size
			);

		for (uint_32 i = 0; i < current_directory.header.file_num; i++) {
			if (current_directory.fcb[i].delete_flag == 1)
				continue;
			printf("  %-25s ", current_directory.fcb[i].name);
			if ((current_directory.fcb[i].attribute & ATT_ARCHIVE) == ATT_ARCHIVE)		printf("�浵\n");
			else if ((current_directory.fcb[i].attribute & ATT_READ) == ATT_READ)	printf("ֻ��\n");
			else if ((current_directory.fcb[i].attribute & ATT_HIDE) == ATT_HIDE)	printf("����\n");
			else if ((current_directory.fcb[i].attribute & ATT_SYSTEM) == ATT_SYSTEM)	printf("ϵͳ\n");
			else printf("\n");
		}
		return 1;
	}

	// ��ʾ�����ļ�����
	for (uint_32 i = 0; i < current_directory.header.file_num; i++) {
		if (current_directory.fcb[i].delete_flag == 1)
			continue;
		if (strcmp(filename, current_directory.fcb[i].name) == 0 && current_directory.fcb[i].delete_flag == 0) {
			if (current_directory.fcb[i].mark == 0)
				printf(" �ļ�����:          %s\n", current_directory.fcb[i].name);
			else
				printf(" �ļ���:            %s\n", current_directory.fcb[i].name);

			printf(" Ȩ��:              ");
			if ((current_directory.fcb[i].attribute & ATT_ARCHIVE) == ATT_ARCHIVE)		printf("�浵\n");
			else if ((current_directory.fcb[i].attribute & ATT_READ) == ATT_READ)	printf("ֻ��\n");
			else if ((current_directory.fcb[i].attribute & ATT_HIDE) == ATT_HIDE)	printf("����\n");
			else if ((current_directory.fcb[i].attribute & ATT_SYSTEM) == ATT_SYSTEM)	printf("ϵͳ\n");
			else printf("\n");

			time = gmtime(&current_directory.fcb[i].create_time);
			printf(" ����ʱ��:          %04d/%02d/%02d  %02d:%02d\n", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, 8 + time->tm_hour, time->tm_min);
			time = gmtime(&current_directory.fcb[i].modify_time);
			printf(" �޸�ʱ��:          %04d/%02d/%02d  %02d:%02d\n", 1900 + time->tm_year, 1 + time->tm_mon, time->tm_mday, 8 + time->tm_hour, time->tm_min);
			printf(" �ļ���С:          %llu\n", current_directory.fcb[i].file_size);
			return 1;
		}
	}

	return -1;
}

