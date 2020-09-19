#include<iostream>
#include<math.h>
#include<fstream>
#include<string>
#include<conio.h>
using std::string;
using std::fstream;
using std::endl;
using std::cout;
using std::cin;

const double e = 2.7182818284590452353602874713527;
static unsigned int count = 0;

int save_count = 0;


bool Xor(int a, int b)
{
	if(a+b == 1)
		return true;
	return false;
}

class Neiron
{
private:
	double learning_rate;
	double Sigmoid(double);
	double Sigmoiddx(double);
	bool created;
	int col;
	double **val;
	int coc;
	int * N;
	double **coef;
	void Correct_Mistackes(double , int , int);
public:
	void Correct_Mistackes(double, int);
	Neiron();
	~Neiron(); 
	void Create_Neiron(const int, int *);
	void Decide(double *);
	void Correct_Mistackes(double *);
	void Show_Neiron();
	void Show_Output();
	void Set();
	void Save_Neiron(string);
	void Load_Neiron(string);
	int Get_max_num()
	{
		int max_num = 0;
		for(int i = 1; i<9; i++)
		{
			cout<<"aaa  "<<i<<endl;
			if(val[col-1][i] > val[col-1][max_num])
			{
				max_num = i;
			}
		}
		return max_num;
	}
	double Get_Output(int);
};

double Neiron::Get_Output(int num)
{
	return val[col-1][num];
}

void Neiron::Load_Neiron(string neiron_name)
{
	this->~Neiron();

	fstream file(neiron_name, std::ios::in);

	file>>created>>col>>coc;

	val = new double* [col];
	coef = new double* [coc];
	N = new int [col];
	for(int i = 0; i<col; i++)
	{
		file>>N[i];
		val[i] = new double [N[i]];
	}

	for(int i = 0; i<coc; i++)
	{
		coef[i] = new double [N[i]*N[i+1]];
	}

	for(int i = 0; i<coc; i++)
	{
		for(int j = 0; j<N[i]*N[i+1]; j++)
		{
			file>>coef[i][j];
		}
	}
	file.close();
}

void Neiron::Save_Neiron(string neiron_name)
{
	if(created == false)
	{
		return;
	}
	fstream file;
	file.open(neiron_name, std::ios::out);
	file<<1<<' ';
	file << col<<' '<<coc<<' ';
	for(int i = 0; i<col; i++)
	{
		file<<N[i]<<" ";
	}
	for(int i = 0; i<coc; i++)
	{
		for(int j = 0; j<N[i]*N[i+1]; j++)
		{
			file<<coef[i][j]<<' ';
		}
	}
	file.close();
}

void Neiron::Correct_Mistackes(double expected, int num)
{
	double error = val[col-1][num] - expected;
	Correct_Mistackes(error, col-1, num);
}

void Neiron::Set()
{
	for(int i = 0; i<coc; i++)
	{
		cout<<"for  coef[ "<<i<<" ] "<<endl;
		for(int j = 0; j < N[i] * N[i+1]; j++)
		{
			cout<<"    coef[i][ "<<j<<" ] = "; cin>>coef[i][j];
		}
	}
}

double Neiron::Sigmoid(double x)
{
	return 1/(1 + pow(e, -x));
}

double Neiron::Sigmoiddx(double x)
{
	return x*(1-x);
}

Neiron::Neiron()
{
	created = false;
	learning_rate = 5;
}

Neiron::~Neiron()
{
	if(created == false)
	{
		return;
	}

	for(int i = 0; i<col; ++i)
	{
		delete [] val[i];
	}
	delete [] val;

	for(int i = 0; i<coc; ++i)
	{
		delete  [] coef[i];
	}
	delete [] coef;
	col = coc = 0;
	created = false;
}

void Neiron::Create_Neiron(const int n, int * x)
{
	if(created == true)
	{
		return;
	}
	created = true;
	col = n;
	val = new double * [n];
	N = new int [n];
	for(int i = 0; i<n; ++i)
	{
		N[i] = x[i];
		val[i] = new double [N[i]];
	}
	
	
	coc = n-1;
	coef = new double* [coc];
	for(int i = 0; i<n-1; ++i)
	{
		coef[i] = new double [N[i] * N[i+1]];
		for(int j = 0; j< N[i] * N[i+1]; ++j)
		{
			coef[i][j] = (rand()%25 - 14.0)/(rand()%13+1.0);
		}
	}
}

void Neiron::Show_Neiron()
{
	if(created == false)
	{
		return;
	}

	cout<<"--------Neiron--------"<<endl;
	cout<<"count of leveles     =  "<<col<<endl
		<<"coeficentes leveles  =  "<<coc<<endl;

	cout<<"coeficentes"<<endl;
	for(int ii = 0; ii<coc; ii++)
	{
		for(int j = 0; j < N[ii]*N[ii+1]; ++j)
		{
			cout<<coef[ii][j]<<"  ";
		}
		cout<<endl;
	}
}

void Neiron::Correct_Mistackes(double error, int lvl_num, int num)
{
	if(lvl_num == 0)
	{
		return;
	}
	double weight_delta = error * Sigmoiddx(val[lvl_num][num]);
	for(int ii = num*N[lvl_num-1]; ii<(num+1)*N[lvl_num-1]; ++ii)
	{
		double delta = weight_delta*learning_rate*val[lvl_num-1][ii%N[lvl_num-1]];
		coef[lvl_num-1][ii] = coef[lvl_num-1][ii] - delta;
		double new_error = weight_delta * coef[lvl_num-1][ii];
		cout<<lvl_num-1<<"  "<<ii%N[lvl_num-1]<<endl;
		//system("pause");
		Correct_Mistackes(new_error , lvl_num -1, ii%N[lvl_num-1]);
	}
}

void Neiron::Correct_Mistackes(double * out)
{
	if(created == false)
	{
		return;
	}
	for(int ii = 0; ii<N[col-1]; ++ii)
	{
		double error = val[col-1][ii] - out[ii];
		cout<<col-1<<"  "<<ii<<endl;
		system("pause");
		Correct_Mistackes(error , col-1, ii);
	}
}

void Neiron::Decide(double * in)
{
	if(created == false)
	{
		return;
	}

	for(int ii = 0; ii<N[0]; ++ii)
	{
		val[0][ii] = in[ii];
	}
	
	for(int ii = 1; ii<col; ++ii) // lvl i hamare
	{
		for(int j = 0; j<N[ii]; ++j)
		{
			double S = 0;
			for(int jj = 0; jj<N[ii-1]; ++jj)
			{
				S += val[ii-1][jj] * coef[ii-1][j * N[ii-1]+jj];
			}
			val[ii][j] = Sigmoid(S);
		}
	}
}

void Neiron::Show_Output()
{
	if(created == false)
	{
		return;
	}

	for(int i = 0; i<N[col-1]; ++i)
	{
		cout<<val[col-1][i]<<"  ";
	}
	cout<<endl;
}


double retwon(double x)
{
	if(x>-1 && x<2)
	{
		return 1;
	}
	return 0;
}

double WinXO(double y[10])
{
	double x[3][3];
	for(int i = 0; i<9; i++)
	{
		x[i/3][i%3] = y[i];
	}
	bool t;
	for(int i = 0; i<3; i++)
	{
		if(x[i][0] == 0)
		{
			continue;
		}
		bool t = true;
		for(int j = 1; j<3 && t; j++)
		{
			if(x[i][0] != x[i][j] )
			{
				t = false;
			}
		}
		if(t == true)
		{
			return retwon(x[i][0] + y[9]);
		}
	}

	for(int i = 0; i<3; i++)
	{
		if(x[0][i] == 0)
		{
			continue;
		}
		bool t = true;
		for(int j = 1; j<3; j++)
		{
			if(x[0][i] != x[j][i])
			{
				t = false;
			}
		}
		if(t == true)
		{
			return retwon(x[0][i] + y[9]);
		}
	}

	if(x[0][0] != 0)
	{
		bool t = true;
		for(int i = 0; i<2 && t; i++)
		{
			if(x[i][i] != x[i+1][i+1])
			{
				t = false;
			}
		}
		if(t)
		{
			return retwon(x[0][0] + y[9]);
		}
	}
	if(x[0][2] != 0)
	{
		bool t = true;
		for(int i = 0; i<2 && t; i++)
		{
			if(x[i][2-i] != x[i+1][2-(i+1)])
			{
				t = false;
			}
		}
		if(t)
		{
			return retwon(x[0][2] + y[9]);
		}
	}
	return 0.5;

}

void Two_to_One(double x[3][3], double y[10])
{
	int k = 0;
	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			y[k++] = x[i][j];
		}
	}
}

void One_to_two(double y[10] , double x[3][3])
{
	for(int i = 0; i<9; i++)
	{
		x[i/3][i%3] = y[i];
	}
}

double invers(double x)
{
	return (1-x);
}

double Max(double a, double b)
{
	if(b>a)
	{
		return b;
	}
	return a;
}

double Min(double a, double b)
{
	if(b<a)
		return b;
	return a;
}

void ShowDouble(double x[3][3])
{

	for(int i = 0; i<3; i++)
	{
		for(int j = 0; j<3; j++)
		{
			if(x[i][j]>0)
			{
				cout<<" X I";
			}
			else if(x[i][j]<0)
			{
				cout<<" O I";
			}
			else
			{
				cout<<"   I"; 
			}
		}
		cout<<endl<<"------------"<<endl;
	}

}
// int max  =  4294967295

double Win(double y[4])
{
	double data [10][5] = 
	{
		1,1,1,0,1,
		1,1,1,1,0,
		-1,-1,-1,0,0,
		-1,-1,-1,1,1,
		0,1,1,0,0.75,
		1,0,1,0,0.75,
		1,1,0,0,0.75,
		0,-1,-1,1,0.75,
		-1,0,-1,1,0.75,
		-1,-1,0,1,0.75,
	};
	for(int i = 0; i<10; i++)
	{
		bool flag = true;
		for(int j = 0; j<4 && flag; j++)
		{
			if(y[j] != data[i][j])
			{
				flag = false;
			}
		}
		if(flag == true)
		{
			return data[i][4];
		}
	}
	return 0.5;
}

void ShowOne(double * p, int count)
{
	for(int i = 0; i<count; i++)
	{
		cout<<p[i]<<"  ";
	}
}

double Win_All(double y[9],int step_num, Neiron & obj)
{
	double out[8] = {0,0,0,0,0,0,0,0};
	double in[4];
	in[3] = step_num;


	in[0] = y[0];
	in[1] = y[1];
	in[2] = y[2];
	obj.Decide(in);
	out[0] = obj.Get_Output(0) - 0.5;

	in[0] = y[3];
	in[1] = y[4];
	in[2] = y[5];
	obj.Decide(in);
	out[1] = obj.Get_Output(0) - 0.5;

	in[0] = y[6];
	in[1] = y[7];
	in[2] = y[8];
	obj.Decide(in);
	out[2] = obj.Get_Output(0) - 0.5;

	in[0] = y[0];
	in[1] = y[3];
	in[2] = y[6];
	obj.Decide(in);
	out[3] = obj.Get_Output(0) - 0.5;

	in[0] = y[1];
	in[1] = y[4];
	in[2] = y[7];
	obj.Decide(in);	
	out[4] = obj.Get_Output(0) - 0.5;

	in[0] = y[2];
	in[1] = y[5];
	in[2] = y[8];
	obj.Decide(in);
	out[5] = obj.Get_Output(0) - 0.5;

	in[0] = y[0];
	in[1] = y[4];
	in[2] = y[8];
	obj.Decide(in);
	out[6] = obj.Get_Output(0) - 0.5;

	in[0] = y[2];
	in[1] = y[4];
	in[2] = y[6]; 
	obj.Decide(in);
	out[7] = obj.Get_Output(0) - 0.5;

	int max_num = 0;
	for(int i = 1; i<8; i++)
	{
		if(fabs(out[max_num]) < fabs(out[i]))
		{
			max_num = i;
		}
	}
	return out[max_num] + 0.5;
}

int max_num(double a, double b)
{
	if(a>b)
		return 0;
	return 1;
}

double Quality(double y[10],int step_num,Neiron &obj,int deep,int num)
{
	if(y[num] != 0)
	{
		return 0;
	}
	y[num] = 1-2*step_num;
	step_num = invers(step_num);
	deep--;
	double ret = Win_All(y,step_num,obj);
	if(ret<0.3 || ret>0.7 || deep==0)
	{
		y[num] = 0;
		return invers(ret);
	}

	double step[9] = {0,0,0,0,0,0,0,0,0};

	for(int i = 0; i<9; i++)
	{
		if(y[i] == 0)
		{
			step[i] = (Quality(y,step_num,obj,deep,i));
		}
	}

	ret = step[0];

	for(int i = 1; i<9; i++)
	{
		if(ret < step[i])
		{
			ret = step[i];
		}
	}
	y[num] = 0;
	return invers(ret);

}


void Do_Step(double y[9], int step_num, Neiron &obj, int deep)

{
	if (deep < 1)
	{
		deep = 1;
	}
	double step[9] = {0,0,0,0,0,0,0,0,0};

	double max_deep = 0;
	for(int i = 0; i<9;i++)
	{
		if(y[i] == 0)
		{
			max_deep++;
		}
	}

	if(deep > max_deep)
	{
		deep = max_deep;
	}

	for(int i = 0; i<9; i++)
	{
		if(y[i] == 0)
		{
			step[i] = Quality(y,step_num,obj,deep,i);
		}
	}
	int max_num = 0;
	for(int i = 1; i<9; i++)
	{
		if(step[max_num] < step[i])
		{
			max_num = i;
		}
	}
	y[max_num] = 1-2*step_num;
}

int main()
{
	Neiron obj;
	obj.Load_Neiron("win");
	double y[10] = {0,0,0,0,0,0,0,0,0,0};
	double x[3][3] = {0,0,0,0,0,0,0,0,0};


	bool step_num = 1;		// 1 play for X ; 0 play for O
	int step = 0;
	int deep = 5;			// complexity unit -> from 1 to 9
	double end;
	do
	{
		system("cls");
		One_to_two(y,x);
		ShowDouble(x);
		if(step%2 == step_num)
		{
			Do_Step(y,step_num,obj,deep);			
		}
		else
		{			
			int a,b;
			cout<<"a = "; cin>>a;
			cout<<"b = "; cin>>b;
			y[3*a+b] = 1-2*invers(step_num);
			
		}
		step++;
		end = Win_All(y,step%2,obj);
		system("cls");
		One_to_two(y,x);
		ShowDouble(x);
	}
	while(end > 0.1 && end < 0.9 && step<9);
	
	system("pause");
	return 0;

}