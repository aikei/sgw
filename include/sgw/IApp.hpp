#ifndef SGW_IAPP_HPP_INCLUDED
#define SGW_IAPP_HPP_INCLUDED

namespace sgw
{
    
struct AppData;
class BaseShape;

class IApp
{
public:
    virtual bool Init(int backend=0)=0;
    virtual void SetAppData(const AppData& _appData)=0;
    virtual const AppData& GetAppData() const=0;
    virtual void Render()=0;
    virtual void ProcessEvents()=0;
    virtual bool IsDone()=0;
    virtual bool SetDone(bool done=true)=0;
    virtual void Draw(const BaseShape& shape)=0;
};


}

#endif
