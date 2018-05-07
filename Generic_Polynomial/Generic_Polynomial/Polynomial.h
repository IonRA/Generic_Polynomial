#pragma once

template <class T>
class pairs;

template <class T>
class polynomial;

template <class T>
std::ostream& operator<<(std::ostream& out, const polynomial<T> &p)
{
	if (p.grade >= 0)
	{
		int i = p.grade;
		if (!i)
			out << p.coeff[i];
		else
			if (i == 1)
			{
				if (p.coeff[1] > 0)
					if (p.coeff[1] == 1)
						out << 'X';
					else
						out << p.coeff[1] << "*X";
				else
					if (p.coeff[1] == -1)
						out << "-X";
					else
						out << p.coeff[1] << "*X";
				if (p.coeff[0] > 0)
					out << '+' << p.coeff[0];
				else
					if (p.coeff[0] < 0)
						out << p.coeff[0];
			}
			else
			{
				if (p.coeff[i] > 0)
					if (p.coeff[i] == 1)
						out << "X^" << i;
					else
						out << p.coeff[i] << "*X^" << i;
				else
					if (p.coeff[i] == -1)
						out << "-X^" << i;
					else
						out << p.coeff[i] << "*X^" << i;
				for (i = p.grade - 1; i > 1; --i)
					if (p.coeff[i] > 0)
						if (p.coeff[i] == 1)
							out << '+' << "X^" << i;
						else
							out << '+' << p.coeff[i] << "*X^" << i;
					else
						if (p.coeff[i] < 0)
							if (p.coeff[i] == -1)
								out << "-X^" << i;
							else
								out << p.coeff[i] << "*X^" << i;
				if (p.coeff[1] > 0)
					if (p.coeff[1] == 1)
						out << "+X";
					else
						out << '+' << p.coeff[1] << "*X";
				else
					if (p.coeff[1] < 0)
						if (p.coeff[1] == -1)
							out << "-X";
						else
							out << p.coeff[1] << "*X";
				if (p.coeff[0] > 0)
					out << '+' << p.coeff[0];
				else
					if (p.coeff[0] < 0)
						out << p.coeff[0];
			}
	}
	else
		out << 0;
	return out;
}

template <class T>
std::ostream& operator<<(std::ostream& out, const pairs<T>& pa)
{
	out << pa.p << '\n';
	out << pa.x;
	return out;
}

template <class T>
std::istream& operator >> (std::istream& in, polynomial<T>& p)
{
	polynomial<T> p2;
	p2.coeff = p.coeff;
	p2.grade = p.grade;
	p.coeff = NULL;
	in >> p.grade;
	if (p.grade < 0)
	{
		p.grade = -1;
		return in;
	}
	T *ptr2 = NULL;
	try
	{
		p.coeff = new T[p.grade + 1];
		for (int i = 0; i <= p.grade; ++i)
			in >> p.coeff[i];
		int g1 = p.grade;
		while (p.grade != -1 && !p.coeff[p.grade])
			--p.grade;
		if (g1 > p.grade)
		{
			if (p.grade != -1)
			{
				ptr2 = new T[p.grade + 1];
				for (g1 = p.grade; g1 >= 0; --g1)
					ptr2[g1] = p.coeff[g1];
			}
			delete[]p.coeff;
			p.coeff = ptr2;
			ptr2 = NULL;
		}
		p.Eisenstein();
	}
	catch (std::exception e)
	{
		delete[]p.coeff;
		delete[]ptr2;
		p.coeff = p2.coeff;
		p.grade = p2.grade;
		p2.coeff = NULL;
		throw e;
	}
	return in;
}

template <class T>
std::istream& operator >> (std::istream& in, pairs<T>& pa)
{
	polynomial<T> p2;
	in >> p2;
	in >> pa.x;
	delete[]pa.p.coeff;
	pa.p.coeff = p2.coeff;
	pa.p.grade = p2.grade;
	p2.coeff = NULL;
	return in;
}

template <class T>
class polynomial
{
protected:
	int grade;
	T* coeff;
	template <class U> friend class polynomial;
	friend class pairs<T>;
	friend std::istream& operator >> <> (std::istream&, polynomial<T>&);
	friend std::ostream& operator<< <>(std::ostream&, const polynomial<T>&);
public:
	polynomial();
	polynomial(const polynomial<T>&);
	template <class U>polynomial(polynomial<U>);
	~polynomial();
	virtual void Eisenstein() {}
	bool Solution(T);
	polynomial<T> operator-();
	polynomial<T> operator+(const polynomial<T>&);
	polynomial<T> operator-(const polynomial<T>&);
	polynomial<T> operator*(const polynomial<T>&);
};

template <class T>
class pairs
{
	T x;
	polynomial<T> p;
	friend std::istream& operator >> <> (std::istream&, pairs<T>&);
	friend std::ostream& operator<< <>(std::ostream&, const pairs<T>&);
public:
	bool Verify();
};

template<class T>
class irred_polynomial :public polynomial<T>
{
public:
	irred_polynomial();
	irred_polynomial(const irred_polynomial<T>&);
	~irred_polynomial();
	void Eisenstein();
};

template <class T>
template <class U>
polynomial<T>::polynomial(polynomial<U>po)
{
	if (po.grade < 0)
		grade = -1, coeff = NULL;
	else
	{
		coeff = new T[po.grade + 1];
		try
		{
			for (int i = 0; i <= po.grade; ++i)
				coeff[i] = po.coeff[i];
		}
		catch (std::exception e)
		{
			delete[]coeff;
			throw e;
		}
		grade = po.grade;
	}
}

template <class T>
polynomial<T>::~polynomial()
{
	delete[] coeff;
}

template <class T>
polynomial<T>::polynomial()
{
	grade = -1;
	coeff = NULL;
}

template <class T>
polynomial<T>::polynomial(const polynomial<T>& p)
{
	if (p.grade >= 0)
	{
		coeff = new T[p.grade + 1];
		grade = p.grade;
		int i;
		try
		{
			for (i = 0; i <= grade; ++i)
				coeff[i] = p.coeff[i];
		}
		catch (std::exception e)
		{
			delete[]coeff;
			throw e;
		}
	}
	else
	{
		grade = -1;
		coeff = NULL;
	}
}

template <class T>
bool polynomial<T>::Solution(T x)
{
	if (grade < 0)
		return true;
	int i;
	T s = coeff[0], p = x;
	for (i = 1; i <= grade; ++i)
	{
		s = s + coeff[i] * p;
		p = p * x;
	}
	if (s)
		return false;
	return true;
}

template <class T>
polynomial<T> polynomial<T>::operator+(const polynomial<T>& p)
{
	if (grade < 0)
		return p;
	if (p.grade < 0)
		return *this;
	polynomial<T> q;
	int i = grade > p.grade ? grade : p.grade;
	q.coeff = new T[i + 1];
	q.grade = i;
	for (i; i >= 0; --i)
	{
		if (i <= grade)
			q.coeff[i] = coeff[i];
		else
			q.coeff[i] = 0;
		if (i <= p.grade)
			q.coeff[i] += p.coeff[i];
	}
	i = q.grade;
	while (q.grade != -1 && !q.coeff[q.grade])
		--q.grade;
	if (q.grade == -1)
	{
		delete[] q.coeff;
		q.coeff = NULL;
	}
	else
		if (i > q.grade)
		{
			T *ptr = new T[q.grade + 1];
			try
			{
				for (i = q.grade; i >= 0; --i)
					ptr[i] = q.coeff[i];
			}
			catch (std::exception e)
			{
				delete[]ptr;
				throw e;
			}
			delete[]q.coeff;
			q.coeff = ptr;
		}
	return q;
}

template <class T>
polynomial<T> polynomial<T>::operator-()
{
	polynomial<T>p(*this);
	int i;
	for (i = grade; i >= 0; --i)
		p.coeff[i] = -p.coeff[i];
	return p;
}

template <class T>
polynomial<T> polynomial<T>::operator-(const polynomial<T>& p)
{
	if (grade < 0)
	{
		polynomial<T>p2(p);
		return (-p2);
	}
	if (p.grade < 0)
		return *this;
	polynomial<T> q;
	int i = grade > p.grade ? grade : p.grade;
	q.coeff = new T[i + 1];
	q.grade = i;
	for (i; i >= 0; --i)
	{
		if (i <= grade)
			q.coeff[i] = coeff[i];
		else
			q.coeff[i] = 0;
		if (i <= p.grade)
			q.coeff[i] -= p.coeff[i];
	}
	i = q.grade;
	while (q.grade != -1 && !q.coeff[q.grade])
		--q.grade;
	if (q.grade == -1)
	{
		delete[] q.coeff;
		q.coeff = NULL;
	}
	else
		if (i > q.grade)
		{
			T *ptr = new T[q.grade + 1];
			try
			{
				for (i = q.grade; i >= 0; --i)
					ptr[i] = q.coeff[i];
			}
			catch (std::exception e)
			{
				delete[]ptr;
				throw e;
			}
			delete[]q.coeff;
			q.coeff = ptr;
		}
	return q;
}

template <class T>
polynomial<T> polynomial<T>::operator*(const polynomial<T>& p)
{
	polynomial<T> s;
	if (grade < 0 || p.grade < 0)
		return s;
	int i;
	int j;
	s.coeff = new T[grade + p.grade + 1];
	s.grade = grade + p.grade;
	for (i = 0; i <= s.grade; ++i)
		s.coeff[i] = 0;
	for (i = grade; i >= 0; --i)
		for (j = p.grade; j >= 0; --j)
			s.coeff[i + j] += coeff[i] * p.coeff[j];
	i = s.grade;
	while (s.grade != -1 && !s.coeff[s.grade])
		--s.grade;
	if (s.grade == -1)
	{
		delete[] s.coeff;
		s.coeff = NULL;
	}
	else
		if (i > s.grade)
		{
			T *ptr = new T[s.grade + 1];
			try
			{
				for (i = s.grade; i >= 0; --i)
					ptr[i] = s.coeff[i];
			}
			catch (std::exception e)
			{
				delete[]ptr;
				throw e;
			}
			delete[]s.coeff;
			s.coeff = ptr;
		}
	return s;
}

template <class T>
bool pairs<T>::Verify()
{
	return p.Solution(x);
}

template <class T>
irred_polynomial<T>::irred_polynomial() :polynomial()
{}

template <class T>
irred_polynomial<T>::irred_polynomial(const irred_polynomial& po) : polynomial(po)
{}

template <class T>
irred_polynomial<T>::~irred_polynomial()
{}

template <class T>
void irred_polynomial<T>::Eisenstein()
{
	if (grade > 1)
	{
		int i;
		int min = abs(coeff[0]);
		int k;
		for (k = 0; k < grade; ++k)
			if (coeff[k] == 1 || coeff[k] == -1)
				throw std::logic_error("The polynomial does not respect the Eisenstein criterion of irreducibility");
			else
				if (coeff[k] && abs(coeff[k]) < min || !min)
					min = abs(coeff[k]);
		if (!min)
			throw std::logic_error("The polynomial does not respect the Eisenstein criterion of irreducibility");//return
		if (coeff[grade] != 1 && coeff[grade] != -1)
		{
			int j = coeff[0] > coeff[1] ? coeff[1] : coeff[0];
			for (i = 2; i*i <= j; ++i)
				if ((int)coeff[0] % i == 0 && (int)coeff[1] % i == 0)
				{
					for (k = 2; k <= grade; ++k)
						if ((int)coeff[k] % i)
							break;
					if (k == grade + 1)
						throw std::logic_error("The polynomial does not respect the Eisenstein criterion of irreducibility");
				}
			for (k = 0; k <= grade; ++k)
				if ((int)coeff[k] % j)
					break;
			if (k == grade + 1)
				throw std::logic_error("The polynomial does not respect the Eisenstein criterion of irreducibility");
		}
		bool b = true;
		k = 2;
		while (k*k <= min)
			if (min%k == 0)
			{
				b = false;
				break;
			}
			else
				++k;
		if (b && (int)coeff[grade] % min && (int)coeff[0] % (min*min))
		{
			for (k = 0; k < grade; ++k)
				if ((int)coeff[k] % min)
					break;
			if (k < grade)
				throw std::logic_error("The polynomial does not respect the Eisenstein criterion of irreducibility");
		}
		else
		{
			while (k*k <= min)
			{
				if (min%k == 0)
				{
					b = true;
					i = 2;
					while (i*i <= k)
						if (k%i == 0)
						{
							b = false;
							break;
						}
						else
							++i;
					if (b && (int)coeff[grade] % k && (int)coeff[0] % (k*k))
					{
						for (i = 0; i < grade; ++i)
							if ((int)coeff[i] % k)
								break;
						if (i == grade)
							return;
					}
				}
				++k;
			}
			throw std::logic_error("The polynomial does not respect the Eisenstein criterion of irreducibility");
		}
	}
}