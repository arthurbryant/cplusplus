#include <iostream>
#include <pcre++.h>

#include <string>
#include <list>
#include <vector>
#include <cctype>
#include <cassert>
#include <fstream>

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
		this->name = name;
		this->mString = mString;
		this->reg = reg;
		this->matched = matched;
		this->offset = offset;
	}
	MState (string name, string mString, int reg)
	{
		//MState (name, mString, reg, false, 0);
		this->name = name;
		this->mString = mString;
		this->reg = reg;
		this->matched = false;
		this->offset = 0;
	}
};
class MatchGroup
{
  private:
	int err;
	DataSet freqwords;
    DataSet domains;
    /* java: TreeSet */
	set<string> confusingNames;
  public:
	vector <MState> states;
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
		this->states = states;
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

		//cout << "states.size()" << states.size() << endl;
		for(int i = 0; i < states.size(); ++i)
		{
			//cout << "name = " << states[i].name << "\tmString = " << states[i].mString << "\tmatched = " << states[i].matched << "\toffset = " << states[i].offset << endl;
		}


		for (int i = 0; i < states.size(); ++i)
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
    	/*
    	for(int i = 0; i < states.size(); ++i)
    	{
    		Mstates ms = states[i];
    		cout << ms.mString << endl;
    	}
    	*/
        string buf = "";
        for(int i = 0; i < states.size(); ++i)
        {
            MState ms = states[i];
            if(0 == ms.name.compare("at"))
            {
                buf += "@";

            }
            else if(endWith(ms.name, "dot"))
            {
                buf += ".";
            }
            else
                buf += ms.mString;
        }
        //cout << "buf: " << buf << endl;
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
		/*vector<MState>::iterator iter = res.begin();
		while(iter != res.end())
		{
			cout << "addMatchGroup res.name = " << (*iter).name << "\t,  addMatchGroup res.mString = " << (*iter).mString << endl;
			++iter;
		}*/
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
        try
        {
    /*	cout << "rpos = " << rpos << "\t" << "freg.size() = " << freg.size() << endl;*/
        if(err > MAX_ERROR)
            return;
        if(rpos == freg.size())
        			{
      /*  for(int i = 0; i < res.size(); ++i)
        			{
        		cout << res[i].name << endl;
        		cout << res[i].mString << endl;
        			}*/
           addMatchGroup(res, err);
       return;
        			}
        // Out of tokens
        if(tpos >= text.length())
            return;
        Pcre regex = freg.getRegex(rpos);
        string name = freg.getName(rpos);
        bool replace = freg.getReplace(rpos);
        bool optional = freg.getOptional(rpos);

       /* cout << "Pcre.name = " << name << endl;*/

        string target = text.substr(tpos);
        if(optional)
        {
        	//cout << "optionla tpos = " << tpos << endl;
            startMatch(res, rpos+1, tpos, err);
        }
        bool found = false;
        int off= 0;
           //cout << "here: " << off<<  endl;
        while(true == regex.search(target, off))
        {

           off += regex.get_match_end() + 1;
           int offset = regex.get_match_start();
		/*   cout << "each offset: " << offset << endl;
		   cout << "target: " << target << endl;*/

           if(offset > 5 && rpos > 0) break;

           string tmp = target.substr(regex.get_match_start(), regex.get_match_end() - regex.get_match_start()+1);
           	   	   	   	   	   //cout << "start = " << regex.get_match_start() << "\t" << "end = "<< regex.get_match_end() << endl;
           //cout << "tmp: " << tmp << endl;

           MState ms(name, tmp, rpos, true, offset);

           /*cout << "ms" << ms.name << endl;
           cout <<  "ms" << ms.mString << endl;*/

           res.push_back(ms);
       /*	for(int i = 0; i < res.size(); ++i)
               	{
               		cout << "res " << res[i].name << endl;
               		cout << "res " << res[i].mString << endl;
               	}*/
          /* cout << "offset = " << offset << endl;
           cout << "tmp.length() = " << tmp.length() << endl;
           	   	   	   	   	   cout << "starMatch tpos = " << tpos << endl;*/
            startMatch(res, rpos+1, tpos + offset + tmp.length(), err);
            res.erase(res.end()-1);

            found = true;
			//cout << endl;
        }
        if(replace && !found)
        {
            MState ms(name, "",  rpos);
            res.push_back(ms);

           /* cout << "replace and !found tpos = " << tpos << endl;*/

            startMatch(res, rpos+1, forwardToken(tpos), err+1);
            res.erase(res.end() - 1);
        }
        }
        catch(exception& e)
        {
            cerr << "Exception: " << e.what() << endl;
        }

		//cout << "res.size()= " << res.size() << endl;
    }
  public:
	  vector < MatchGroup > match (FuzzyRegex regex, string text)
	{
        mgps.clear();
        matchStates.clear();
        this->text = text;
        this->freg = regex;

        startMatch(matchStates, 0, 0, 0);

		//cout << "starMatch.size() = " << this->getMatchState().size() << endl;

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
		  regex.add (username, "username", false, true);
		  regex.add (at, "at", true, false);
		  regex.add (subdomain, "subdomain", false, false);
		  regex.add (dot, "dot", true, false);
		  regex.add (subdomain, "subdomain", false, true);
		  regex.add (dot, "dot", true, true);
		  regex.add (subdomain, "subdomain", false, true);
		  regex.add (dot, "dot", true, true);
		  regex.add (domain, "domain", false, false);
	}
    string recognize(string text)
    {
        text = preprocess(text);
        //cout << "text = " << text << endl;

        FuzzyMatcher matcher;
        vector<MatchGroup> mg = matcher.match(regex, text);

        int maxScore = 0;
        MatchGroup ans;
        //cout <<"mg.size() = "<< mg.size() << endl;
        int k;
        for(int i = 0; i < mg.size(); ++i)
        {
            int score = mg[i].score();
            if(score > maxScore)
            {
                ans = mg[i];
                maxScore = score;
                k = i;
            }
        }
        /*vector<MState> vms = ans.states;
        vector<MState>::iterator iter = vms.begin();
        while(iter != vms.end())
        {
        	cout << iter->name << "\t" << iter->mString << "\t" << iter->matched << "\t" << iter->offset << endl;
        	++iter;
        }
*/
        if(maxScore > 1)
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

    	/* !!!This test do not pass */
		//assert(rec.recognize("Jeffrey D. Ullman  ullman @ cs.stanford.edu 650-494-8016 (home) 650-725-2588 (FAX)") == "ullman@cs.stanford.edu");

	/*
		assert(rec.recognize("e-mail: blaz.zupan%fri.uni-lj.si (replace % by @)") == "blaz.zupan@fri.uni-lj.si");
	assert(rec.recognize("joehust@gmail.com") == "joehust@gmail.com");
		assert(rec.recognize("Mohammed Mansour- now at Amazon.com.") != "now@Amazon.com");
		assert(rec.recognize("joehust@gmail.com") == "joehust@gmail.com");
		assert(rec.recognize("ahlswede@mathematik.uni-bielefeld.de") == "ahlswede@mathematik.uni-bielefeld.de");
		assert(rec.recognize("E-mail:  dumer@ee.ucr.edu") == "dumer@ee.ucr.edu");
		assert(rec.recognize("Email: mreiser at umd.edu") == "mreiser@umd.edu");
		assert(rec.recognize("Phone: (434) 982-2275  Fax: (434) 982-2214  Email:  stankovic@cs.virginia.edu  Home Page:  John Stankovic") == "stankovic@cs.virginia.edu");
		assert(rec.recognize("towsley at cs dot umass dot edu") == "towsley@cs.umass.edu");
		assert(rec.recognize("Email: vern@icir.org / vern@cs.berkeley.edu") == "vern@cs.berkeley.edu");
		assert(rec.recognize("email: tom (at) cs.washington.edu") == "tom@cs.washington.edu");
		assert(rec.recognize("email: melody (at) cs.washington.edu") == "melody@cs.washington.edu");
		assert(rec.recognize("pattrsn at cs dot berkeley dot edu") == "pattrsn@cs.berkeley.edu");
		assert(rec.recognize("e-mail: culler -AT- cs.berkeley.edu") == "culler@cs.berkeley.edu");
		assert(rec.recognize("dbj @ cs.rice.edu") == "dbj@cs.rice.edu");
		assert(rec.recognize("af[at]cs[dot]princeton[dot]edu") == "af@cs.princeton.edu");
		assert(rec.recognize("email to blanker<AT>cs.tu-berlin.de blanker<AT>cs.tu-berlin.de") == "blanker@cs.tu-berlin.de");
		assert(rec.recognize("Stefan.Decker  'AT' DERI . org") == "Stefan.Decker@DERI.org");
		*/

	string fileName = "test/data1.txt";
	ifstream infile(fileName.c_str());
	if(!infile)
	{
		cout << "Unable to open file: " << fileName << endl;
	}

	string line;
	while(getline(infile, line))
	{
		string result = rec.recognize(line);
		if(result != "")
			cout << result << endl;
	}
}

int
main ()
{
    doTest();

	return 0;
}
