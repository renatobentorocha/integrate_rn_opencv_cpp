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
  load: (imageUrl: string, exif: number) => Promise<void>;
  toGray: () => Promise<void>;
  connectedComponents: () => Promise<void>;
  toOriginal: () => Promise<void>;
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

  function load(imageUrl: string, exif: number) {
    NativeLibModule.load(imageUrl, exif);
  }

  function toGray() {
    NativeLibModule.toGray();
  }

  function connectedComponents() {
    NativeLibModule.connectedComponents();
  }

  function toOriginal() {
    NativeLibModule.toOriginal();
  }

  function subscribeToUpdate(handler: (event: UpdateSubscription) => void) {
    return NativeLibEmmiter.addListener(Event.UpdatedImage, handler);
  }

  return {
    init,
    load,
    toGray,
    connectedComponents,
    toOriginal,
    subscribeToUpdate,
  };
}
