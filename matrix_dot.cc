#include <iostream>
#include <string>
using namespace std;

void matrixDot(string &s, string &t)
{
    cout << "s:" << s << '\n';
    cout << "t:" << t << '\n';
    int rows, cols;
    rows = s.size();
    cols = t.size();

    cout << "╔";
    for (int i = 0; i < rows * 2; i++)
        cout << "═";
    cout << "╗";

    cout << '\n';
    for (int i = 0; i < rows; i++)
    {
        cout << "║";
        for (int j = 0; j < cols; j++)
        {
            if (s[i] == t[j])
                cout << "o ";
            else
                cout << "  ";
        }
        cout << "║\n";
    }



    cout << "╚";
    for (int i = 0; i < rows * 2; i++)
        cout << "═";
    cout << "╝";
}

int main()
{
    string s, t;
    s = "atggaagcaatatcactgatgactatactactggtggtaacaacaagtaatgcagacaaaatctgcatcggtcaccaatcaacaaattccacggaaactgtagacacgctaacagaaacaa";
    t = "attaaaggtttataccttcccaggtaacaaaccaaccaactttcgatctcttgtagatctgttctctaaacgaactttaaaatctgtgtggctgtcactcggctgcatgcttagtgcactcacgcagtataattaataactaattactgtcgttgacaggacacgagtaactcgtctatcttctgcaggctgcttacggtttcgtccgtgttgcagccgatcatcagcacatctaggtttcgtccgggtgtgaccgaaaggtaagatggagagccttgtccctggtttcaacgagaaaacacacgtccaactcagtttgcctgttttacaggttcgcgacgtgctcgtacgtggctttggagactccgtggaggaggtcttatcagaggcacgtcaacatcttaaagatggcacttgtggcttagtagaagttgaaaaaggcgttttgcctcaacttgaacagccctatgtgttcatcaaacgttcggatgctcgaactgcacctcatggtcatgttatggttgagctggtagcagaactcgaaggcattcagtacggtcgtagtggtgagacacttggtgtccttgtccctcatgtgggcgaaataccagtggcttaccgcaaggttcttcttcgtaagaacggtaataaaggagctggtggccatagttacggcgccgatctaaagtcatttgacttaggcgacgagcttggcactgatccttatgaagattttcaagaaaactggaacactaaacatagcagtggtgttacccgtgaactcatgcgtgagcttaacggaggggcatacactcgctatgtcgataacaacttctgtggccctgatggctaccctcttgagtgcattaaagaccttctagcacgtgctggtaaagcttcatgcactttgtccgaacaactggactttattgacactaagaggggtgtatactgctgccgtgaacatgagcatgaaattgcttggtacacggaacgttctgaaaagagctatgaattgcagacaccttttgaaattaaattggcaaagaaatttgacaccttcaatggggaatgtccaaa";

    s = s.substr(0, 40);
    t = t.substr(0, 40);

    matrixDot(s, t);
}