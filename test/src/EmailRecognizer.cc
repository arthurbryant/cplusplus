#include <iostream>
#include <pcre++.h>

#include <string>
#include <list>
#include <vector>
#include <cctype>
#include <cassert>

#include "header.h"

using namespace std;
using namespace pcrepp;

class FuzzyRegex
{
  private:
	vector < Pcre > regexVector;
	vector < string > names;
	vector < bool > replaceAttr;
	vector < bool > optionalAttr;

  public:
	void add (Pcre regex, string name, bool replace, bool optional)
	{
		regexVector.push_back (regex);
		names.push_back (name);
		replaceAttr.push_back (replace);
		optionalAttr.push_back (optional);
	}
	Pcre getRegex (int index)
	{
		return regexVector[index];
	}
	string getName (int index)
	{
		return names[index];
	}
	bool getReplace (int index)
	{
		return replaceAttr[index];
	}
	bool getOptional (int index)
	{
		return optionalAttr[index];
	}
	int size ()
	{
		return regexVector.size ();
	}

};
class MState
{
  public:
	string name;
	string mString;
	int reg;
	int mtype;
	bool matched;
	int offset;

	  MState (string name, string mString, int reg, bool matched,
			  int offset)
	{
		name = name;
		mString = mString;
		reg = reg;
		matched = matched;
		offset = offset;
	}
	MState (string name, string mString, int reg)
	{
		MState (name, mString, reg, false, 0);
	}
};
class MatchGroup
{
  private:
	vector <MState> states;
	int err;
	DataSet freqwords; 
    DataSet domains; 
    /* java: TreeSet */
	set<string> confusingNames;
  public:
    MatchGroup() 
	{
	    KnowlegeBase kb;
        freqwords = kb.getDictionary("freq-word-list");
        domains = kb.getDictionary("domain-name");

		confusingNames.insert("dot");
		confusingNames.insert("stop");
		confusingNames.insert("at");
		confusingNames.insert("period");
	}
	MatchGroup (vector < MState > states, int err)
	{
		states = states;
		this->err = err;

	    KnowlegeBase kb;
        freqwords = kb.getDictionary("freq-word-list");
        domains = kb.getDictionary("domain-name");

		confusingNames.insert("dot");
		confusingNames.insert("stop");
		confusingNames.insert("at");
		confusingNames.insert("period");
	}
	vector<MState> getMatchState()
	{
		return states;
	}
	int score ()
	{

		int NAME_NLP_PENALTY = 10;
		int MALFORMAL_NAME_PENALTY = 20;
		int AT_MATCH_BONUS = 5;
		int CONFUSE_NAME_PENALITY = 20;
		int NLP_SUBDOMAIN_PENALITY = 3;
		int PATTERN_PENALITY = 20;
		int DOT_ALLMATCH_BONUS = 10;
		int DOMAIN_MATCH_BONUS = 10;
		int DOMAIN_NLP_PENALITY = 7;
		int ERROR_PENALITY = 3;
		int FUZZY_PENALITY = 5;

		int score = 0;

		int unmatchedDot = 0;

		for (int i = 0; i < states.size (); ++i)
		{
			MState mst = states[i];
			Pcre numberRegex("\\d+");

			if (0 == mst.name.compare("username"))
			{
				if (numberRegex.search(mst.mString))
				{
					score -= MALFORMAL_NAME_PENALTY;
				}
				else if (mst.mString.length () < 2)
				{
					score -= MALFORMAL_NAME_PENALTY;
				}
				else if (confusingNames.count(allToLower(mst.mString)))
				{
					score -= MALFORMAL_NAME_PENALTY;
				}
				else if (freqwords.lookup (mst.mString))
				{
					score -= NAME_NLP_PENALTY;
				}
			}
			else if (0 == mst.name.compare("at"))
			{
				if (mst.matched)
				{
					score += AT_MATCH_BONUS;
				}
			}
			else if (0 == mst.name.compare ("subdomain"))
			{
				if (confusingNames.count(allToLower(mst.mString)))
				{
					score -= CONFUSE_NAME_PENALITY;
				}

				if (freqwords.lookup (mst.mString))
				{
					score -= NLP_SUBDOMAIN_PENALITY;
				}

				score += 1;

			}
			else if (0 == mst.name.compare ("dot"))
			{
				if (!mst.matched)
				{
					unmatchedDot++;
				}
				else
				{
					if (mst.offset > 2)
						score -= FUZZY_PENALITY;
				}
			}
			else if (0 == mst.name.compare("domain"))
			{
				if (domains.lookup (mst.mString))
				{
					score += DOMAIN_MATCH_BONUS;
				}

				if (freqwords.lookup (mst.mString))
				{
					score -= DOMAIN_NLP_PENALITY;
				}
			}

			// check legal pattern
			if (i > 0)
			{
				MState prev = states[i - 1];

				if (0 == mst.name.compare ("username")
					&& !(0 == prev.name.compare ("udot")))
				{
					score -= PATTERN_PENALITY;
				}
				else if (0 == mst.name.compare ("dot")
						 && !(0 == prev.name.compare ("subdomain")))
				{
					score -= PATTERN_PENALITY;
				}
				else if (0 == mst.name.compare ("domain")
						 && !(0 == prev.name.compare ("dot")))
				{
					score -= PATTERN_PENALITY;
				}
				else if (0 == mst.name.compare ("at")
						 && !(0 == prev.name.compare ("username")))
				{
					score -= PATTERN_PENALITY;
				}
			}


		}

		// macro judge
		if (unmatchedDot == 0)
		{
			score += DOT_ALLMATCH_BONUS;
		}

		score -= err * ERROR_PENALITY;

		return score;
	}
	string toString()
	{
		string buf = "";
		for(int i = 0; i < states.size(); ++i)
		{
			MState ms = states[i];
			buf += "(";
			buf += ms.name;
			buf += (")");
			buf += ms.mString;
		}
		buf += " : " + err;
		buf += " : " + score();

		return buf;
	}
    string convertString()
    {
        string buf = "";
        for(int i = 0; i < states.size(); ++i)
        {
            MState ms = states[i];
            if(0 == ms.name.compare("at"))
            {
                buf += "@";
            }
            /* ??? */
            else if(ms.name.find_last_of("dot") != -1)
            {
                buf += ".";
            }
            else
                buf += ms.mString;
        }
        return buf;
    }
};
class FuzzyMatcher
{
  private:
	static const int MAX_ERROR = 2;
	FuzzyRegex freg;
	  vector < MState > matchStates;
	  vector < MatchGroup > mgps;

	string text;
    void addMatchGroup(vector<MState> &res, int err)
    {
		vector<MState>::iterator iter = res.begin();
		while(iter != res.end())
		{
			cout << (*iter).name << (*iter).mString << endl;
			++iter;
		}

        MatchGroup mg(res, err);
        mgps.push_back(mg);
    }
    int forwardToken(int start)
    {
		int it = start;
		while (it < text.length()) {
			char c = text[it];
			if (isalpha(c) || isdigit(c)) 
            {
				++it;
			}
            else break;
		}
		return it;
    }
    void startMatch(vector<MState> &res, int rpos, int tpos, int err)
    {
        if(err > MAX_ERROR) 
            return;
        if(rpos == freg.size())
        {
            addMatchGroup(res, err);
			cout << "aaa: " << res.size() << endl;
            return;
        }

        // Out of tokens
        if(tpos >= text.length())
            return;
        Pcre regex = freg.getRegex(rpos);
        string name = freg.getName(rpos);
        bool replace = freg.getReplace(rpos);
        bool optional = freg.getOptional(rpos);

        //cout << name << endl;

        string target = text.substr(tpos);
        //cout << target << endl;
        if(optional)
        {
            startMatch(res, rpos+1, tpos, err);
        }
        bool found = false;
        int off= 0;
           //cout << "here: " << off<<  endl;
        try
        {
        while(true == regex.search(target, off))
        {
           off += regex.get_match_end() + 1;
           int offset = regex.get_match_start();
		   cout << "each offset: " << offset << endl;
		   cout << "target: " << target << endl;
           if(offset > 5 && rpos > 0) break;
           
           string tmp = target.substr(regex.get_match_start(), regex.get_match_end()+1);

           cout << "tmp: " << tmp << endl;

           MState ms(name, tmp, rpos, true, offset);
           res.push_back(ms);

            startMatch(res, rpos+1, tpos + offset + tmp.length(), err);
            res.erase(res.end()-1);
            
            found = true;
			cout << endl;
        }
        if(replace && !found)
        {
			cout << "jump to replace && !found " << endl;
            MState ms(name, "",  rpos);
            res.push_back(ms);
            startMatch(res, rpos+1, forwardToken(tpos), err+1);
            res.erase(res.end() - 1);
        }
        }
        catch(exception& e)
        {
            cerr << "Exception: " << e.what() << endl;
        }
		
		cout << "res.size()= " << res.size() << endl;
    }
  public:
	  vector < MatchGroup > match (FuzzyRegex regex, string text)
	{
        mgps.clear();
        matchStates.clear();
        this->text = text;
        this->freg = regex;

        startMatch(matchStates, 0, 0, 0);

		cout << "starMatch.size() = " << this->getMatchState().size() << endl;

        return mgps;
	}
	  vector<MState> getMatchState()
	  {
		return matchStates;
	  }

};
class EmailRecognizer
{
  private:
	FuzzyRegex regex;
  public:
	EmailRecognizer ()
	{
		string usernameString = "[A-Za-z\\d][A-Za-z\\d_-]*[A-Za-z\\d]";
		string atString = "@|at";
		string dotString = "\\.|dot|period|stop";
		string subdomainString =
			"[\\dA-Za-z][\\dA-Za-z-]*[\\dA-Za-z]";
		string domainString = "[a-z]{2,4}";
		Pcre username (usernameString);
		Pcre at (atString, "i");
		Pcre dot (dotString, "i");
		Pcre subdomain (subdomainString);
		Pcre domain (domainString);

		  regex.add (username, "username", false, false);
		  regex.add (dot, "udot", false, true);
		  regex.add (username, " username", false, true);
		  regex.add (at, "at", true, false);
		  regex.add (subdomain, " subdomain", false, false);
		  regex.add (dot, "dot", true, false);
		  regex.add (subdomain, " subdomain", false, true);
		  regex.add (dot, "dot", true, true);
		  regex.add (subdomain, " subdomain", false, true);
		  regex.add (dot, "dot", true, true);
		  regex.add (domain, "domain", false, false);
	}
    string recognize(string text)
    {
        text = preprocess(text);

        FuzzyMatcher matcher;
        vector<MatchGroup> mg = matcher.match(regex, text);

        int maxScore = 0;
        MatchGroup ans;
        for(int i = 0; i < mg.size(); ++i)
        {
			/*
			vector<MState> tmp = mg[i].getMatchState;
			vector<MState>::iterator iter = tmp.begin();
			while(iter != tmp.end())
			{
				cout << iter->name << endl;
				++iter;
			}
			*/

            int score = mg[i].score();
            if(score > maxScore)
            {
                ans = mg[i];
                maxScore = score;
            }
        }
		cout << "recognize success" << endl;
		cout << "maxScore " << maxScore << endl;

        if(maxScore > 18)
            return ans.convertString();

       	return ""; 
    }
    string preprocess(string rawEmailText)
    {
        Pcre emailPrefix("e-?mail:?", "i");
        if(true == emailPrefix.search(rawEmailText)) 
        {
            rawEmailText = rawEmailText.substr(emailPrefix.get_match_end() + 1);
        }

        return rawEmailText;
    }
	string getName ()
	{
		return "email";
	}
	FuzzyRegex getRegex ()
	{
		return regex;
	}
};
static void doTest()
{
    EmailRecognizer rec;
	string temp = rec.recognize("joehust@gmail.com");
	cout << rec.recognize("joehust@gmail.com") << endl;
	cout << "" << endl;
	//assert(rec.recognize("joehust@gmail.com") ==  string("joehust@gmail.com"));
}
int
main ()
{
    doTest();
    /*
	EmailRecognizer *er = new EmailRecognizer ();
	FuzzyRegex fr = er->getRegex ();

	Pcre regex;
	string name;
	bool replace;
	bool optional;

	for (int i = 0; i < fr.size (); ++i)
	{
		regex = fr.getRegex (i);
		name = fr.getName (i);
		replace = fr.getReplace (i);
		optional = fr.getOptional (i);

		cout << "\t" << name << "\t" << replace << "\t" << optional <<
			endl;
	}
	cout << er->getName () << endl;
    */
	return 0;
}
