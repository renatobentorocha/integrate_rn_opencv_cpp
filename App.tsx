import React, {useCallback, useEffect, useState} from 'react';
import {Image, StyleSheet, Text, TouchableOpacity, View} from 'react-native';
import ImagePicker from 'react-native-image-crop-picker';
import {useNativeLib} from './useNativeLib';

export default function App() {
  const [image, setImage] = useState('');
  const nativeLib = useNativeLib();

  useEffect(() => {
    ImagePicker.clean();
  }, []);

  useEffect(() => {
    nativeLib.init();
    const subscribe = nativeLib.subscribeToUpdate(subscribeToUpdate);

    return () => subscribe.remove();
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, []);

  const subscribeToUpdate = useCallback(
    async (event: {image_base_64: string}) => {
      console.log(event.image_base_64);
      setImage(`data:image/jpeg;base64,${event.image_base_64}`);
    },
    [],
  );

  function handleOriginal() {
    nativeLib.toOriginal();
  }

  function handleGray() {
    nativeLib.toGray();
  }

  function handleConnectedComponents() {
    nativeLib.connectedComponents();
  }

  async function openPicker() {
    const res = await ImagePicker.openPicker({
      mediaType: 'photo',
      includeExif: true,
    });

    let Orientation = '';

    if (res.exif) {
      Orientation = (res.exif as {Orientation: string}).Orientation;
    }

    nativeLib.load(res.path, parseInt(Orientation, 10));
  }

  function getActionButton() {
    if (image) {
      return (
        <View style={styles.buttonWrapper}>
          <TouchableOpacity style={[styles.button]} onPress={handleOriginal}>
            <Text style={styles.text}>To begin</Text>
          </TouchableOpacity>
          <TouchableOpacity style={[styles.button]} onPress={handleGray}>
            <Text style={styles.text}>To gray</Text>
          </TouchableOpacity>
          <TouchableOpacity
            style={styles.button}
            onPress={handleConnectedComponents}>
            <Text style={styles.text}>Connected components</Text>
          </TouchableOpacity>
        </View>
      );
    }

    return (
      <TouchableOpacity style={[styles.button]} onPress={openPicker}>
        <Text style={styles.text}>Picker</Text>
      </TouchableOpacity>
    );
  }

  return (
    <View style={styles.container}>
      <View style={styles.container}>
        <Image
          style={StyleSheet.absoluteFill}
          source={{uri: image || undefined}}
        />
      </View>
      {getActionButton()}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
  },
  buttonWrapper: {
    flexDirection: 'row',
  },
  button: {
    borderWidth: 1,
    borderColor: '#444',
    flex: 1,
    maxHeight: 55,
    height: 55,
    backgroundColor: '#565292',
    alignItems: 'center',
    justifyContent: 'center',
  },
  text: {
    color: '#fff',
  },
});
