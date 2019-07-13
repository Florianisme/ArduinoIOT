package com.florianisme.arduinoiot;

public class Plant {

    private final int layoutId;
    private final String firebaseId;

    public Plant(int layoutId, String firebaseId) {
        this.layoutId = layoutId;
        this.firebaseId = firebaseId;
    }

    public String getFirebaseId() {
        return firebaseId;
    }


    public int getLayoutId() {
        return layoutId;
    }

}
