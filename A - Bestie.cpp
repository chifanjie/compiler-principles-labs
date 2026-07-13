#include<bits/stdc++.h>
using namespace std;
int a[100];
int main()
{
	int t; cin >> t;
	while (t--)
	{
		int n; cin>>n;
		int gcdd;
		for (int i = 1; i <= n; i++)
		{
			cin>>a[i];
			if (i == 1)
				gcdd = a[i];
			if (i >= 2)
			{
				gcdd = __gcd(gcdd, a[i]);
			}
		}
		if (gcdd == 1)
		{
			cout<<"0"<<endl;
			continue;
		}
		else
		{
			bool flag = 0;
			for (int i = n; i >= n-1; i--)
			{
				if (__gcd(i, gcdd) == 1)
				{
					cout<<n-i+1<<endl;
					flag = 1;
					break;
				}
			}
			if(!flag)
                cout<<"3"<<endl;
		}
	}
	return 0;
}
