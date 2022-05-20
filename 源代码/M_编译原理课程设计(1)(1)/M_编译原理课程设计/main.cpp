//#include<bits/stdc++.h>
#include <iostream>
#include"sematic_analysis.h"
using namespace std;
int main(int argc, char* argv[])
{
	class Semantic S(argv[2]);
	//cout << S.file_name << endl;
	/**for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "digui")==0)cout << "yes" << endl;
	}**/
	if (strcmp(argv[1], "digui") == 0)
		S.TestDigui();
	else if(strcmp(argv[1], "ll1") == 0)
		S.TestLL1();
	system("pause");
}
