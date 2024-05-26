#include <queue>
#include <vector>

using namespace std;

template <class T>
class AhoCorasickAutomaton
{
public:
	static const int sigma = 26;

	class Node
	{
	public:
		int len;
		int val;
		int slnk;
		int plnk;
		int son[sigma];

		Node(void) : len(0), val(-1), slnk(0), plnk(0)
		{
			memset(son, 0, sizeof son);
		}
	};

	int sz;
	vector<Node> nodes;

	int idx(const T & x)
	{
		return x - 'a';
	}

	AhoCorasickAutomaton(void)
	{
		clear();
	}

	Node & operator [] (const int & x)
	{
		return nodes[x];
	}

	void clear(void)
	{
		sz = 0;
		nodes.clear();
		add_node();
	}

	int add_node(void)
	{
		nodes.push_back(Node());
		return sz++;
	}

	int insert(const T * first, const T * last, const int & id)
	{
		const T * & s = first;
		const int n = last - first;

		int u = 0;
		for (int i = 0; i < n; i++)
		{
			const int c = idx(s[i]);
			int v = nodes[u].son[c];
			if (!v)
			{
				v = add_node();
				nodes[u].son[c] = v;
				nodes[v].len = nodes[u].len + 1;
			}
			u = v;
		}
		nodes[u].val = id;
		return u;
	}

	void build(void)
	{
		queue<int> que;
		for (int c = 0; c < sigma; c++)
		{
			const int & u = nodes[0].son[c];
			if (u != 0)
			{
				que.push(u);
				nodes[u].slnk = 0;
				nodes[u].plnk = 0;
			}
		}
		for (; !que.empty() ; que.pop())
		{
			const int & r = que.front();
			for (int c = 0; c < sigma; c++)
			{
				int & u = nodes[r].son[c], v = nodes[r].slnk;
				if (u == 0)
					u = nodes[v].son[c];
				else
				{
					que.push(u);
					v = nodes[u].slnk = nodes[v].son[c];
					nodes[u].plnk = nodes[v].val != -1 ? v : nodes[v].plnk;
				}
			}
		}
	}

	void upd(const int & u)
	{
		if (u != 0)
		{
			if (nodes[u].val >= 0)
			{
//				func(nodes[u].val);
			}
			upd(nodes[u].plnk);
		}
	}

	void calc(const T * first, const T * last)
	{
		const T * & t = first;
		const int n = last - first;
		for (int i = 0, k = 0; i < n; i++)
		{
			int c = idx(t[i]);
			k = nodes[k].son[c];
			upd(k);
		}
	}

	vector<int> sorted_indices(void)
	{
		int mxlen = 0;
		for (int u = 0; u < sz; u++)
			mxlen = max(mxlen, nodes[u].len);
		vector<int> occ(mxlen + 1);
		for (int u = 0; u < sz; u++)
			occ[nodes[u].len]++;
		for (int i = 1; i <= mxlen; i++)
			occ[i] += occ[i - 1];
		vector<int> ans(sz);
		for (int u = 0; u < sz; u++)
			ans[--occ[nodes[u].len]] = u;
		return ans;
	}

	vector<int> sorted_indices(const vector<int> & qry)
	{
		int mxlen = 0;
		for (const auto & u : qry)
			mxlen = max(mxlen, nodes[u].len);
		vector<int> occ(mxlen + 1);
		for (const auto & u : qry)
			occ[nodes[u].len]++;
		for (int i = 1; i <= mxlen; i++)
			occ[i] += occ[i - 1];
		vector<int> ans(qry.size());
		for (const auto & u : qry)
			ans[--occ[nodes[u].len]] = u;
		return ans;
	}
};

