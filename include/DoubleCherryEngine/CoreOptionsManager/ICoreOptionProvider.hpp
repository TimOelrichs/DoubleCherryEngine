class ICoreOptionProvider {
protected:
    virtual void getOptionDefinitions() {}
    virtual void getOptionsCategories() {}

    virtual void onOptionsUpdate() {}

};
