import {
  NativeModules,
  NativeEventEmitter,
  EmitterSubscription,
} from 'react-native';

export enum Event {
  'UpdatedImage' = 'UpdatedImage',
}

type UpdateSubscription = {
  image_base_64: string;
};

type NativeLibProps = {
  init: () => void;
  toGray: (imageUrl: string) => Promise<void>;
  subscribeToUpdate: (
    handler: (event: UpdateSubscription) => void,
  ) => EmitterSubscription;
};

const NativeLibModule = NativeModules.NativeLibModule as NativeLibProps;
const NativeLibEmmiter = new NativeEventEmitter(NativeModules.NativeLibModule);

export function useNativeLib() {
  function init() {
    NativeLibModule.init();
  }

  function toGray(imageUrl: string) {
    NativeLibModule.toGray(imageUrl);
  }

  function subscribeToUpdate(handler: (event: UpdateSubscription) => void) {
    return NativeLibEmmiter.addListener(Event.UpdatedImage, handler);
  }

  return {
    init,
    toGray,
    subscribeToUpdate,
  };
}
