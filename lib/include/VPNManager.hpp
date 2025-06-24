#include <vector>

enum class VPNType
{
    ZeroTire
};

enum class VPNStatus {
    NotFound,
    Working
};

class IVPN
{
public:
    virtual ~IVPN() = default;
    virtual void connect();
private:
    std::vector<>
};


class VPNManager
{

};