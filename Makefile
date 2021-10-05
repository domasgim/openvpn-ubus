include $(TOPDIR)/rules.mk

PKG_NAME:=openvpn-ubus
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/openvpn-ubus
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=openvpn-ubus
	DEPENDS:=+libubus +libubox +libblobmsg-json +libuci +libargp
endef

define Package/openvpn-ubus/description
	OpenVPN ubus management system
endef

define Package/openvpn-ubus/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/openvpn-ubus $(1)/usr/bin/openvpn-ubus
	$(INSTALL_BIN) ./files/openvpn.init $(1)/etc/init.d/openvpn
	$(INSTALL_BIN) ./files/openvpn-ubus.init $(1)/etc/init.d/openvpn-ubus
endef

$(eval $(call BuildPackage,openvpn-ubus))
