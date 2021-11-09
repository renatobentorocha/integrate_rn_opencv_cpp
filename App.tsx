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
      setImage(`data:image/jpeg;base64,${event.image_base_64}`);
    },
    [],
  );

  function handleGray() {
    nativeLib.toGray(image);
  }

  async function openPicker() {
    const res = await ImagePicker.openPicker({
      mediaType: 'photo',
      includeExif: true,
    });

    setImage(res.path);
  }

  function getActionButton() {
    if (image) {
      return (
        <TouchableOpacity style={styles.button} onPress={handleGray}>
          <Text style={styles.text}>To gray</Text>
        </TouchableOpacity>
      );
    }

    return (
      <TouchableOpacity style={styles.button} onPress={openPicker}>
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
  button: {
    height: 55,
    backgroundColor: '#565292',
    alignItems: 'center',
    justifyContent: 'center',
  },
  text: {
    color: '#fff',
  },
});
