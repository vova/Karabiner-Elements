#import "DeviceManager.h"
#import "NotificationKeys.h"
#import "libkrbn.h"

@interface KarabinerKitDeviceManager ()

@property libkrbn_connected_devices_monitor* libkrbn_connected_devices_monitor;
@property(readwrite) KarabinerKitConnectedDevices* connectedDevices;

@end

static void connected_devices_updated_callback(libkrbn_connected_devices* initializedConnectedDevices,
                                               void* refcon) {
  KarabinerKitDeviceManager* manager = (__bridge KarabinerKitDeviceManager*)(refcon);
  manager.connectedDevices = [[KarabinerKitConnectedDevices alloc] initWithInitializedConnectedDevices:initializedConnectedDevices];

  [[NSNotificationCenter defaultCenter] postNotificationName:kKarabinerKitDevicesAreUpdated object:nil];
}

@implementation KarabinerKitDeviceManager

+ (KarabinerKitDeviceManager*)sharedManager {
  static dispatch_once_t once;
  static KarabinerKitDeviceManager* manager;
  dispatch_once(&once, ^{
    manager = [KarabinerKitDeviceManager new];
  });

  return manager;
}

- (instancetype)init {
  self = [super init];

  if (self) {
    libkrbn_connected_devices_monitor* p = NULL;
    if (libkrbn_connected_devices_monitor_initialize(&p, connected_devices_updated_callback, (__bridge void*)(self))) {
      _libkrbn_connected_devices_monitor = p;
    }
  }

  return self;
}

- (void)dealloc {
  if (self.libkrbn_connected_devices_monitor) {
    libkrbn_connected_devices_monitor* p = self.libkrbn_connected_devices_monitor;
    libkrbn_connected_devices_monitor_terminate(&p);
  }
}

@end
