#ifndef LSA_H
#define LSA_H

#define INIT_SEQ_NUM    0x80000001

class Link
{
public:
    Link(){};
    Link(int src_id, int dst_id)
    {
        this->SRC_ID = src_id;
        this->DST_ID = dst_id;
    }

    bool operator< (const Link& rhs) const
    {
        if (this->SRC_ID < rhs.SRC_ID)
        {
            return true;
        } else if (this->SRC_ID > rhs.SRC_ID)
        {
            return false;
        } else
        {
            // lhs.SRC_ID == rhs.SRC_ID
            if (this->DST_ID < rhs.DST_ID)
            {
                return true;
            } else if (this->DST_ID >= rhs.DST_ID)
            {
                return false;
            }
        }
    }
private:
    int SRC_ID;
    int DST_ID;
};

struct RouterLSA
{
    // SRC = Advertising Router ID
    // DST = Link ID (only for Router LSAs)
    Link LINK;
    long SEQ_NUM;
    int LINK_COST;
};

#endif