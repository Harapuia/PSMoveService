#ifndef SERVER_DEVICE_VIEW_H
#define SERVER_DEVICE_VIEW_H

//-- includes -----
#include "DeviceInterface.h"
#include <assert.h>

// -- declarations -----
class ServerDeviceView
{
public:
    ServerDeviceView(const int device_id);
    virtual ~ServerDeviceView();
    
    virtual bool open(const class DeviceEnumerator *enumerator);
    bool poll();
    virtual void updateStateAndPredict()= 0;
    void publish();
    void close();
    bool matchesDeviceEnumerator(const class DeviceEnumerator *enumerator) const;
    
    // getters
    inline int getDeviceID() const
    { return m_deviceID; }
    
    virtual IDeviceInterface* getDevice() const=0;
    
    // Used for when you have to get device specific data
    template <class t_controller_subclass>
    inline const t_controller_subclass *castCheckedConst() const
    { 
        IDeviceInterface* device= getDevice();
        assert(device != nullptr);
        assert(device->getDeviceType() == t_controller_subclass::getDeviceTypeStatic());
        const t_controller_subclass *controller= static_cast<const t_controller_subclass *>(device);

        return controller; 
    }
    template <class t_controller_subclass>
    inline t_controller_subclass *castChecked()
    {
        IDeviceInterface* device= getDevice();
        assert(device != nullptr);
        assert(device->getDeviceType() == t_controller_subclass::getDeviceTypeStatic());
        t_controller_subclass *controller= static_cast<t_controller_subclass *>(device);

        return controller; 
    }    

    // Returns true if device opened successfully
    bool getIsOpen() const;
    inline bool getHasUnpublishedState()
    { return m_bHasUnpublishedState; }
    
    // setters
    inline void markStateAsUnpublished()
    { m_bHasUnpublishedState= true; }
    inline void setDeviceID(int id)
    { m_deviceID= id; }
    
protected:
    virtual bool allocate_device_interface(const class DeviceEnumerator *enumerator) = 0;
    virtual void free_device_interface() = 0;
    virtual void publish_device_data_frame() = 0;

    bool m_bHasUnpublishedState;
    int m_pollNoDataCount;
    int m_sequence_number;
    
private:
    int m_deviceID;
};

#endif // SERVER_DEVICE_VIEW_H
