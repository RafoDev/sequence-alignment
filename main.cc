#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>
#include <filesystem>

// Neddleman-Wunsch

#define inf std::numeric_limits<int>::min();

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;

const int penaltyScore = 2;

using namespace std;

struct Node
{
  int dir;
  Node *children[3];
  int nChilds;
  Node(int _dir)
  {
    dir = _dir;
    nChilds = 0;
    children[0] = nullptr;
    children[1] = nullptr;
    children[2] = nullptr;
  }
  Node()
  {
    dir = -1;
    nChilds = 0;
    children[0] = nullptr;
    children[1] = nullptr;
    children[2] = nullptr;
  }
  void addChild(Node *child)
  {
    children[nChilds++] = child;
  }
  void setDir(int _dir)
  {
    dir = _dir;
  }
  bool hasNotDir()
  {
    return dir == -1;
  }
};

void traceback(int i, int j, vvi &dp, string &s, string &t, Node *root)
{
  if (i == 0 && j == 0)
    return;
  else
  {
    Node *currNode = new Node();

    int diag, up, left, max;
    bool match = s[i - 1] == t[j - 1];

    diag = up = left = inf;

    if (i > 0)
    {
      if (j > 0)
      {
        diag = dp[i - 1][j - 1] + (match ? +1 : -1);
        left = dp[i - 1][j] - penaltyScore;
        up = dp[i][j - 1] - penaltyScore;
      }
      else
      {
        left = dp[i - 1][j] - penaltyScore;
      }
    }
    else
    {
      up = dp[i][j - 1] - penaltyScore;
    }

    max = std::max(diag, up);
    max = std::max(max, left);

    if (diag == max)
    {
      currNode->setDir(match ? 0 : 1);
      traceback(i - 1, j - 1, dp, s, t, currNode);
    }
    if (up == max)
    {
      if (currNode->hasNotDir())
      {
        currNode->setDir(2);
        traceback(i, j - 1, dp, s, t, currNode);
      }
      else
      {
        Node *newBranch = new Node(2);
        traceback(i, j - 1, dp, s, t, newBranch);
        root->addChild(newBranch);
      }
    }
    if (left == max)
    {
      if (currNode->hasNotDir())
      {
        currNode->setDir(3);
        traceback(i - 1, j, dp, s, t, currNode);
      }
      else
      {
        Node *newBranch = new Node(3);
        traceback(i - 1, j, dp, s, t, newBranch);
        root->addChild(newBranch);
      }
    }

    root->addChild(currNode);
  }
}

void getPathsUtil(Node *node, std::vector<int> &currentPath, std::vector<std::vector<int>> &allPaths)
{
  if (node == nullptr)
    return;

  currentPath.push_back(node->dir);

  if (node->nChilds == 0)
  {
    allPaths.push_back(currentPath);
  }
  else
  {
    for (int i = 0; i < node->nChilds; ++i)
    {
      getPathsUtil(node->children[i], currentPath, allPaths);
    }
  }

  currentPath.pop_back();
}

void getPaths(vvi &dp, string &s, string &t)
{
  int i, j;
  i = s.size();
  j = t.size();

  Node *root = new Node();
  traceback(i, j, dp, s, t, root);

  std::vector<std::vector<int>> allPaths;
  std::vector<int> currentPath;

  getPathsUtil(root, currentPath, allPaths);

  cout << "s:" << s << "\n";
  cout << "t:" << t << "\n";
  cout << "Aligments:\n";
  for (const auto &path : allPaths)
  {
    int pathLength = path.size();
    int ti = 0;
    int si = 0;
    for (int dir = pathLength - 1; dir >= 1; dir--)
    {
      if (path[dir] == 2)
        cout << "_";
      else
        cout << s[si++];
    }
    cout << '\n';
    for (int dir = pathLength - 1; dir >= 1; dir--)
    {
      if (path[dir] == 3)
        cout << "_";
      else
        cout << t[ti++];
    }
    std::cout << std::endl;
  }
}

int computeMax(int i, int j, vvi &dp, vvi &path, char si, char ti)
{
  int diag, vGap, hGap, max;
  bool match = si == ti;

  diag = dp[i - 1][j - 1] + (match ? +1 : -1);
  vGap = dp[i - 1][j] - penaltyScore;
  hGap = dp[i][j - 1] - penaltyScore;

  max = std::max(diag, vGap);
  max = std::max(max, hGap);

  return max;
}

void show(vvi &dp)
{
  for (auto v : dp)
  {
    for (auto i : v)
    {
      std::cout << i << ' ';
    }
    std::cout << '\n';
  }
}

void nw(std::string &s, std::string &t)
{
  int rows, cols;
  rows = s.size();
  cols = t.size();

  std::vector<std::vector<int>> dp, path;

  dp.resize(rows + 1, std::vector<int>(cols + 1, 0));
  path.resize(rows + 1, std::vector<int>(cols + 1, 0));

  for (int i = 1; i <= rows; i++)
  {
    dp[i][0] = -penaltyScore * i;
    path[i][0] = 1;
  }
  for (int j = 1; j <= cols; j++)
  {
    dp[0][j] = -penaltyScore * j;
    path[0][j] = 3;
  }

  // show(dp);

  for (int i = 1; i <= rows; i++)
  {
    for (int j = 1; j <= cols; j++)
    {
      dp[i][j] = computeMax(i, j, dp, path, s[i - 1], t[j - 1]);
      // show(dp);
      // std::cout << '\n';
    }
  }
  cout << "Se armo\n";

  getPaths(dp, s, t);
}

vector<string> getFilenames(string &dir)
{
  vector<string> filenames;
  for (const auto &entry : filesystem::directory_iterator(dir))
  {
    if (entry.is_regular_file())
      filenames.push_back(entry.path());
  }
  return filenames;
}

unordered_map<string, string> getSequences(vector<string> &filenames)
{
  unordered_map<string, string> sequences;

  for (auto filename : filenames)
  {
    fstream file(filename);
    string word, sequence, name;

    file >> name;

    while (file >> word)
    {
      if (word.size() == 10)
        sequence += word;
    }
    sequences[name] = sequence;
  }
  return sequences;
}

void printSequences(unordered_map<string, string> &sequences)
{
  for (auto seq : sequences)
  {
    cout << "Name: " << seq.first << '\n';
    cout << "Length: " << seq.second.size() << '\n';
    cout << "Sequence: " << seq.second << "\n\n";
  }
}

int main()
{
  string dir = "../sequences";
  vector<string> filenames = getFilenames(dir);
  unordered_map<string, string> sequences = getSequences(filenames);
  printSequences(sequences);

  std::string s, t;
  s = "tcaagcgttagagaagtcattatgtgataaaaaaattcaacttggtatcaacttaactaa";
  t = "attaaaggtttataccttcccaggtaacaaaccaaccaactttcgatctcttgtagatct";
  nw(s, t);
  // nw(sequences["Bacteria"], sequences["Sars-Cov"]);
}