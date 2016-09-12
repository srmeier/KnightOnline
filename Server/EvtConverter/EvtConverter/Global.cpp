/*
*/

//-----------------------------------------------------------------------------
int ParseSpace(char* tBuf, char* sBuf) {
	int i = 0, index = 0;
	bool flag = false;

	while(sBuf[index]==' ' || sBuf[index] =='\t')index++;
	while(sBuf[index]!=' ' && sBuf[index]!='\t' && sBuf[index]!='\0') {
		tBuf[i++] = sBuf[index++];
		flag = true;
	}
	tBuf[i] = 0;

	while(sBuf[index]==' ' || sBuf[index]=='\t')index++;

	if(!flag) return 0;

	return index;
}
