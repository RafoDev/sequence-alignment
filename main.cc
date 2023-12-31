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
#include <chrono>
// Neddleman-Wunsch

#define inf std::numeric_limits<int>::min();

typedef std::vector<int> vi;
typedef std::vector<vi> vvi;
const std::string RESET_COLOR = "\033[0m";
const std::string RED_TEXT = "\033[31m";
const std::string GREEN_TEXT = "\033[32m";
const std::string YELLOW_TEXT = "\033[33m";
const std::string BLUE_TEXT = "\033[34m";
const std::string MAGENTA_TEXT = "\033[35m";
const std::string CYAN_TEXT = "\033[36m";
const std::string BOLD_TEXT = "\033[1m";
const int penaltyScore = 2;
typedef long long ll;

using namespace std;

void printAlignment(vector<int> &path, string &s, string &t, int tam)
{
  int pathLength = tam - 1;
  int ti = 0;
  int si = 0;
  cout << "s: ";
  for (int dir = pathLength; dir >= 0; dir--)
  {
    if (path[dir] == 2)
      cout << "_";
    else
      cout << s[si++];
  }
  cout << '\n';
  cout << "t: ";
  for (int dir = pathLength; dir >= 0; dir--)
  {
    if (path[dir] == 3)
      cout << "_";
    else
      cout << t[ti++];
  }
  cout << "\n";
}

inline int max3(int a, int b, int c)
{
  return max(max(a, b), c);
}

void traceback(int i, int j, vvi &dp, string &s, string &t, vector<int> &path, ll &count, int &index)
{
  if (i == 0 && j == 0)
  {
    // "Código para imprimir el alineamiento"
    cout << "[" << ++count << "]\n";
    printAlignment(path, s, t, index);
    cout << '\n';

    // "Código para imprimir los pasos"
    // cout << "[" << ++count << "] ";
    // for (auto dir : path)
    //   cout << dir;
    // cout << '\n';

    // "Código para contar los alineamientos"
    // ++count;

    return;
  }
  else
  {
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

    max = max3(diag, up, left);

    if (diag == max)
    {
      path[index++] = !match;
      traceback(i - 1, j - 1, dp, s, t, path, count, index);
      index--;
    }
    if (up == max)
    {
      path[index++] = 2;
      traceback(i, j - 1, dp, s, t, path, count, index);
      index--;
    }
    if (left == max)
    {
      path[index++] = 3;
      traceback(i - 1, j, dp, s, t, path, count, index);
      index--;
    }
  }
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

  std::vector<std::vector<int>> dp;
  dp.resize(rows + 1, std::vector<int>(cols + 1, 0));

  for (int i = 1; i <= rows; i++)
    dp[i][0] = -penaltyScore * i;

  for (int j = 1; j <= cols; j++)
    dp[0][j] = -penaltyScore * j;

  auto start = chrono::high_resolution_clock::now();

  for (int i = 1; i <= rows; i++)
    for (int j = 1; j <= cols; j++)
      dp[i][j] = max3(
          dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? +1 : -1),
          dp[i - 1][j] - penaltyScore,
          dp[i][j - 1] - penaltyScore);

  cout<<'\n';

  auto stop = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> dpTime = stop - start;

  cout << BOLD_TEXT << GREEN_TEXT << "[ INFO | Original Sequences ]\n"
       << RESET_COLOR;
  cout << "s: " << s << '\n';
  cout << "t: " << t << "\n\n";

  vector<int> tmpPath(rows + cols);
  ll count = 0;
  int index = 0;

  cout << BOLD_TEXT << GREEN_TEXT << "[ INFO | Possible Alignments ]\n"
       << RESET_COLOR;
  start = chrono::high_resolution_clock::now();
  traceback(rows, cols, dp, s, t, tmpPath, count, index);
  stop = chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> seqTime = stop - start;

  cout << BOLD_TEXT << GREEN_TEXT << "[ INFO | Summary]\n";
  cout << "max score: " << dp[s.size()][t.size()] <<'\n'; 
  cout << "n of alignments: " << count <<'\n'; 
  cout<< "score matrix: "<< dpTime.count() << " milliseconds\n";
  cout<< "traceback: "<<seqTime.count()<<" milliseconds\n";
  cout<<"total time: "<<dpTime.count() + seqTime.count()<<" milliseconds\n";
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
  s = sequences["Bacteria"];
  t = sequences["Sars-Cov"];
  // s = "AAAC";
  // t = "AGC";
  nw(s, t);
}