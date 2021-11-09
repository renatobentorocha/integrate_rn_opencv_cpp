import {
  NativeModules,
  NativeEventEmitter,
  EmitterSubscription,
} from 'react-native';

export enum Event {
  'UpdatedImage' = 'UpdatedImage',
}

type UpdateSubscription = {
  image64: string;
};

type NativeLibProps = {
  toGray: (imageUrl: string) => Promise<void>;
  subscribeToUpdate: (
    handler: (event: UpdateSubscription) => void,
  ) => EmitterSubscription;
};

const NativeLibModule = NativeModules.NativeLibModule as NativeLibProps;
const NativeLibEmmiter = new NativeEventEmitter(NativeModules.NativeLibModule);

export function useNativeLib() {
  function toGray(imageUrl: string) {
    NativeLibModule.toGray(imageUrl);
  }

  function subscribeToUpdate(handler: (event: UpdateSubscription) => void) {
    return NativeLibEmmiter.addListener(Event.UpdatedImage, handler);
  }

  return {
    toGray,
    subscribeToUpdate,
  };
}
