/*
 Navicat Premium Data Transfer

 Source Server         : BDO
 Source Server Type    : SQL Server
 Source Server Version : 15002000 (15.00.2000)
 Source Host           : 192.168.20.253:1433
 Source Catalog        : SA_BETA_WORLDDB_0002
 Source Schema         : PaWebPublic

 Target Server Type    : SQL Server
 Target Server Version : 15002000 (15.00.2000)
 File Encoding         : 65001

 Date: 05/03/2023 15:10:15
*/


-- ----------------------------
-- Table structure for link
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[link]') AND type IN ('U'))
	DROP TABLE [PaWebPublic].[link]
GO

CREATE TABLE [PaWebPublic].[link] (
  [id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NOT NULL,
  [title] varchar(255) COLLATE Chinese_PRC_CI_AS  NULL,
  [link] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [create_date] datetime2(7)  NULL,
  [create_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [create_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_date] datetime2(7)  NULL,
  [update_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL
)
GO

ALTER TABLE [PaWebPublic].[link] SET (LOCK_ESCALATION = TABLE)
GO


-- ----------------------------
-- Records of link
-- ----------------------------
INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'0a99ed3581fa40af8232f72543610e83', N'wechat', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'305f1d2254c14100af7f63cd3e706ae5', N'qq', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'5c933f3a33f74237a1302eb6f67e2ae0', N'github', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'7dd1bfa106aa47f1aa77873d8bfad4e5', N'nvidia', N'https://www.nvidia.com/Download/index.aspx', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'9355a61456fa4b65b84fd701b57938b9', N'launcher', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'9f68e0ebf3e142b88884a571a18b4221', N'weibo', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'c98e44ad43374d99bfaff330ac0d19dd', N'client', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'dc461eb2c75c461a8802f66089db6e55', N'telegram', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-21 01:16:07.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'dcf2a515db9e4755aa256bd39288376a', N'discord', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO

INSERT INTO [PaWebPublic].[link] ([id], [title], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id]) VALUES (N'e90a5801e9ff4c8891cb92b8a4484269', N'amd', N'https://www.amd.com/en/support', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin')
GO


-- ----------------------------
-- Table structure for news
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[news]') AND type IN ('U'))
	DROP TABLE [PaWebPublic].[news]
GO

CREATE TABLE [PaWebPublic].[news] (
  [id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NOT NULL,
  [title] varchar(255) COLLATE Chinese_PRC_CI_AS  NULL,
  [date] datetime2(7)  NULL,
  [author] varchar(255) COLLATE Chinese_PRC_CI_AS  NULL,
  [content] varchar(4096) COLLATE Chinese_PRC_CI_AS  NULL,
  [create_date] datetime2(7)  NULL,
  [create_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [create_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_date] datetime2(7)  NULL,
  [update_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [delete] int  NULL
)
GO

ALTER TABLE [PaWebPublic].[news] SET (LOCK_ESCALATION = TABLE)
GO


-- ----------------------------
-- Records of news
-- ----------------------------
INSERT INTO [PaWebPublic].[news] ([id], [title], [date], [author], [content], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [delete]) VALUES (N'0779b846cd4649448fac3eb858256cd2', N'测试2', N'2023-02-20 00:00:00.0000000', N'测试2', N'5rWL6K+VMjMzMw==', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'1')
GO

INSERT INTO [PaWebPublic].[news] ([id], [title], [date], [author], [content], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [delete]) VALUES (N'3719eb98ff334eae9fcdf66676943bab', N'测试新闻1', N'2023-02-19 14:32:50.0000000', N'测试作者1', N'TWFya2VkIC0gTWFya2Rvd24gUGFyc2VyCj09PT09PT09PT09PT09PT09PT09PT09PQoKW01hcmtlZF0gbGV0cyB5b3UgY29udmVydCBbTWFya2Rvd25dIGludG8gSFRNTC4gIE1hcmtkb3duIGlzIGEgc2ltcGxlIHRleHQgZm9ybWF0IHdob3NlIGdvYWwgaXMgdG8gYmUgdmVyeSBlYXN5IHRvIHJlYWQgYW5kIHdyaXRlLCBldmVuIHdoZW4gbm90IGNvbnZlcnRlZCB0byBIVE1MLiAgVGhpcyBkZW1vIHBhZ2Ugd2lsbCBsZXQgeW91IHR5cGUgYW55dGhpbmcgeW91IGxpa2UgYW5kIHNlZSBob3cgaXQgZ2V0cyBjb252ZXJ0ZWQuICBMaXZlLiAgTm8gbW9yZSB3YWl0aW5nIGFyb3VuZC4KCkhvdyBUbyBVc2UgVGhlIERlbW8KLS0tLS0tLS0tLS0tLS0tLS0tLQoKMS4gVHlwZSBpbiBzdHVmZiBvbiB0aGUgbGVmdC4KMi4gU2VlIHRoZSBsaXZlIHVwZGF0ZXMgb24gdGhlIHJpZ2h0LgoKVGhhdCdzIGl0LiAgUHJldHR5IHNpbXBsZS4gIFRoZXJlJ3MgYWxzbyBhIGRyb3AtZG93biBvcHRpb24gYWJvdmUgdG8gc3dpdGNoIGJldHdlZW4gdmFyaW91cyB2aWV3czoKCi0gKipQcmV2aWV3OioqICBBIGxpdmUgZGlzcGxheSBvZiB0aGUgZ2VuZXJhdGVkIEhUTUwgYXMgaXQgd291bGQgcmVuZGVyIGluIGEgYnJvd3Nlci4KLSAqKkhUTUwgU291cmNlOioqICBUaGUgZ2VuZXJhdGVkIEhUTUwgYmVmb3JlIHlvdXIgYnJvd3NlciBtYWtlcyBpdCBwcmV0dHkuCi0gKipMZXhlciBEYXRhOioqICBXaGF0IFttYXJrZWRdIHVzZXMgaW50ZXJuYWxseSwgaW4gY2FzZSB5b3UgbGlrZSBnb3J5IHN0dWZmIGxpa2UgdGhpcy4KLSAqKlF1aWNrIFJlZmVyZW5jZToqKiAgQSBicmllZiBydW4tZG93biBvZiBob3cgdG8gZm9ybWF0IHRoaW5ncyB1c2luZyBtYXJrZG93bi4KCldoeSBNYXJrZG93bj8KLS0tLS0tLS0tLS0tLQoKSXQncyBlYXN5LiAgSXQncyBub3Qgb3Zlcmx5IGJsb2F0ZWQsIHVubGlrZSBIVE1MLiAgQWxzbywgYXMgdGhlIGNyZWF0b3Igb2YgW21hcmtkb3duXSBzYXlzLAoKPiBUaGUgb3ZlcnJpZGluZyBkZXNpZ24gZ29hbCBmb3IgTWFya2Rvd24ncwo+IGZvcm1hdHRpbmcgc3ludGF4IGlzIHRvIG1ha2UgaXQgYXMgcmVhZGFibGUKPiBhcyBwb3NzaWJsZS4gVGhlIGlkZWEgaXMgdGhhdCBhCj4gTWFya2Rvd24tZm9ybWF0dGVkIGRvY3VtZW50IHNob3VsZCBiZQo+IHB1Ymxpc2hhYmxlIGFzLWlzLCBhcyBwbGFpbiB0ZXh0LCB3aXRob3V0Cj4gbG9va2luZyBsaWtlIGl0J3MgYmVlbiBtYXJrZWQgdXAgd2l0aCB0YWdzCj4gb3IgZm9ybWF0dGluZyBpbnN0cnVjdGlvbnMuCgpSZWFkeSB0byBzdGFydCB3cml0aW5nPyAgRWl0aGVyIHN0YXJ0IGNoYW5naW5nIHN0dWZmIG9uIHRoZSBsZWZ0IG9yCltjbGVhciBldmVyeXRoaW5nXSgvZGVtby8/dGV4dD0pIHdpdGggYSBzaW1wbGUgY2xpY2suCgpbTWFya2VkXTogaHR0cHM6Ly9naXRodWIuY29tL21hcmtlZGpzL21hcmtlZC8KW01hcmtkb3duXTogaHR0cDovL2RhcmluZ2ZpcmViYWxsLm5ldC9wcm9qZWN0cy9tYXJrZG93bi8=', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'0')
GO

INSERT INTO [PaWebPublic].[news] ([id], [title], [date], [author], [content], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [delete]) VALUES (N'bb933da796904362956c2dc80a5916e2', N'3506客户端下载链接', N'2023-02-21 00:00:00.0000000', N'admin', N'ICAgICAgICAgICAgICAgICAgIOeZvuW6puS6keebmOmTvuaOpSAgCmh0dHBzOi8vcGFuLmJhaWR1LmNvbS9zLzFGQTZlcDJZRVg0M2xibWE5ZzRSU2xBP3B3ZD02MjE5wqAKICAK5o+Q5Y+W56CBOjYyMTk=', N'2023-02-21 01:04:02.0000000', N'WebAdmin', N'admin', N'2023-02-21 01:13:38.0000000', N'WebAdmin', N'admin', N'0')
GO


-- ----------------------------
-- Table structure for shop
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[shop]') AND type IN ('U'))
	DROP TABLE [PaWebPublic].[shop]
GO

CREATE TABLE [PaWebPublic].[shop] (
  [id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NOT NULL,
  [title] varchar(255) COLLATE Chinese_PRC_CI_AS  NULL,
  [price] money  NULL,
  [storage] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [img] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [desc] varchar(255) COLLATE Chinese_PRC_CI_AS  NULL,
  [create_date] datetime2(7)  NULL,
  [create_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [create_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_date] datetime2(7)  NULL,
  [update_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL
)
GO

ALTER TABLE [PaWebPublic].[shop] SET (LOCK_ESCALATION = TABLE)
GO


-- ----------------------------
-- Records of shop
-- ----------------------------

-- ----------------------------
-- Table structure for slide
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[slide]') AND type IN ('U'))
	DROP TABLE [PaWebPublic].[slide]
GO

CREATE TABLE [PaWebPublic].[slide] (
  [id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NOT NULL,
  [title] varchar(255) COLLATE Chinese_PRC_CI_AS  NULL,
  [desc] varchar(255) COLLATE Chinese_PRC_CI_AS  NULL,
  [link] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [create_date] datetime2(7)  NULL,
  [create_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [create_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_date] datetime2(7)  NULL,
  [update_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [img] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL
)
GO

ALTER TABLE [PaWebPublic].[slide] SET (LOCK_ESCALATION = TABLE)
GO


-- ----------------------------
-- Records of slide
-- ----------------------------
INSERT INTO [PaWebPublic].[slide] ([id], [title], [desc], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [img]) VALUES (N'15a74cd82abe4a83a244e86a96708bf3', N'测试滚动图3', N'测试滚动图3简介', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'http://1.13.22.82:51530/imgs/slide/3.jpg')
GO

INSERT INTO [PaWebPublic].[slide] ([id], [title], [desc], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [img]) VALUES (N'562f075ffee44fc3bae2f3a31e308b8a', N'测试滚动图4', N'测试滚动图4简介', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'http://1.13.22.82:51530/imgs/slide/4.jpg')
GO

INSERT INTO [PaWebPublic].[slide] ([id], [title], [desc], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [img]) VALUES (N'62e80b3dc3774ac0af9f33a9c783f650', N'测试滚动图5', N'测试滚动图5简介', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'http://1.13.22.82:51530/imgs/slide/5.jpg')
GO

INSERT INTO [PaWebPublic].[slide] ([id], [title], [desc], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [img]) VALUES (N'6391c00835b944ea96a60ba93912ee42', N'测试滚动图1', N'测试滚动图1简介', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'http://1.13.22.82:51530/imgs/slide/1.jpg')
GO

INSERT INTO [PaWebPublic].[slide] ([id], [title], [desc], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [img]) VALUES (N'f71f4a3c1c604407a7901d61dc59648c', N'测试滚动图2', N'测试滚动图2简介', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'http://1.13.22.82:51530/imgs/slide/2.jpg')
GO


-- ----------------------------
-- Table structure for version
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[version]') AND type IN ('U'))
	DROP TABLE [PaWebPublic].[version]
GO

CREATE TABLE [PaWebPublic].[version] (
  [id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NOT NULL,
  [name] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [version] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [force] int  NULL,
  [link] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [create_date] datetime2(7)  NULL,
  [create_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [create_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_date] datetime2(7)  NULL,
  [update_user] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [update_id] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [delete] int  NULL,
  [oldPath] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [newPath] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL,
  [savePath] varchar(255) COLLATE SQL_Latin1_General_CP1_CI_AS  NULL
)
GO

ALTER TABLE [PaWebPublic].[version] SET (LOCK_ESCALATION = TABLE)
GO


-- ----------------------------
-- Records of version
-- ----------------------------
INSERT INTO [PaWebPublic].[version] ([id], [name], [version], [force], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [delete], [oldPath], [newPath], [savePath]) VALUES (N'9e7c9c33a3ca467b8ad0aed1f12ba5f2', N'app', N'2023.02.019', N'0', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'0', N'', N'', N'')
GO

INSERT INTO [PaWebPublic].[version] ([id], [name], [version], [force], [link], [create_date], [create_user], [create_id], [update_date], [update_user], [update_id], [delete], [oldPath], [newPath], [savePath]) VALUES (N'fbe6d98e191a43d49b9f95a8960b6916', N'resource', N'2023.02.019', N'0', N'', N'2023-02-20 17:51:03.0000000', N'WebAdmin', N'admin', N'2023-02-20 17:51:08.0000000', N'WebAdmin', N'admin', N'0', N'', N'', N'')
GO


-- ----------------------------
-- procedure structure for uspBlockUserByUserIdForDaum
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[uspBlockUserByUserIdForDaum]') AND type IN ('P', 'PC', 'RF', 'X'))
	DROP PROCEDURE[PaWebPublic].[uspBlockUserByUserIdForDaum]
GO

CREATE PROCEDURE [PaWebPublic].[uspBlockUserByUserIdForDaum]
	 @userId		NVARCHAR(30)	
	,@blockEndTime	DATETIME
	,@blockCode		INT
	,@blockMemo		NVARCHAR(200)
	--,@symNo			VARCHAR(50)     OUTPUT			-- 실패한경우에만의미있음(GameCommonSymNo.h에기술되어야함)
	-- 다음 웹에서 sqlrelay를 통해서 호출하는데 return 은 레코드셋으로만 받을 수 있고 output 파라미터로 받을 수 없어서 삭제
AS
/*!
 *  \brief		운영툴 : 유저Id로 유저를 블록/해제 한다(다음 웹)
 *  \author		황지현
--1. 제재 적용 시 코드 상/하위 여부 관계없이 end date 기준 더 오래 제한되는 제한 적용
--2. 단, 동일 제재코드 적용이 진행될 경우, 중복제재이기에 적용 불가
--3. 이용제한 해제는 이전 제재 내역에 관계없이 "무조건" 적용
--4. 모든 이용제한과 해지 적용내용은 히스토리 확인 가능해야함
 */
BEGIN
	SET NOCOUNT ON										-- Count-set결과를생성하지말아라.
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	SET LOCK_TIMEOUT 3000								-- 함부로변경금지, 변경필요시DBA와협의.
	SET XACT_ABORT ON
             
	DECLARE @rv		INT = 0								-- 0:성공, 음수:오류코드, 양수:사용하면안됨(ODBC오류로처리함)

	/**********************************************************************************************
	** 작업시작(이하에만기술)
	** 코딩 규정 : JOIN 구문을 사용할 경우 반드시 별칭을 사용하며 컬럼명앞에 해당 별칭을 기술합니다
	**********************************************************************************************/
	BEGIN TRAN	 uspBlockUserByUserIdForDaum_Tx1	-- 트랜잭션 이름과 순서를 명시한다
	
	DECLARE @userNo	BIGINT                             
	SELECT @userNo = _userNo 
		FROM PaGamePrivate.TblUserInformation		 
		WHERE _userId = @userId
	IF(1 <> @@ROWCOUNT)
		 BEGIN
			SET @rv		= -1
			--SET @symNo	= 'eErrNoDbInternalError'
			GOTO LABEL_END	
		 END

	-- 제재 해제일 경우 시간 무관하게 무조건 적용
	IF(1000 = @blockCode)
	 BEGIN
		-- 기록이 이미 있을 경우 업데이트한다
		UPDATE PaGamePrivate.TblBlockedUser 
		SET	_registerDate	= PaGamePublic.ufnGetCustomDatetime()
			,_endDate		= @blockEndTime
			,_blockCode		= @blockCode
			,_operationMemo	= @blockMemo
		WHERE (_userNo = @userNo)

		IF(0 = @@ROWCOUNT) --기록이 없을 경우
		 BEGIN
			INSERT PaGamePrivate.TblBlockedUser	(_registerDate, _userNo,	_endDate,		_blockCode,	_operationMemo)
										VALUES	(PaGamePublic.ufnGetCustomDatetime(),		@userNo,	@blockEndTime,	@blockCode,	@blockMemo)	
			IF(1 <> @@ROWCOUNT)
			 BEGIN
				SET @rv		= -2
				--SET @symNo	= 'eErrNoDbInternalError'
				GOTO LABEL_END	
			 END		
		 END

	 END		
	ELSE -- 제재 해제가 아닐 경우
	 BEGIN
		 -- 해당 유저 기록이 이미 있으면
		IF EXISTS ( SELECT * FROM PaGamePrivate.TblBlockedUser WHERE (_userNo = @userNo) )
		 BEGIN
			UPDATE PaGamePrivate.TblBlockedUser 
			SET	_registerDate	= PaGamePublic.ufnGetCustomDatetime()
				,_endDate		= @blockEndTime
				,_blockCode		= @blockCode
				,_operationMemo	= @blockMemo
			WHERE (_userNo = @userNo) AND (_endDate < @blockEndTime) AND (_blockCode <> @blockCode)
			
			IF(0 = @@ROWCOUNT) -- 해당 유저 기록은 있지만 남은 _endDate가 더 길거나 _blockCode가 기존과 같은 중복제재이다
			 BEGIN
				SET @rv		= -1001
				--SET @symNo	= 'eErrNoDbInternalError'
				GOTO LABEL_END	
			 END		
		 END
		ELSE --제재 해제도 아니고 이전 기록도 없는 경우
		 BEGIN
			INSERT PaGamePrivate.TblBlockedUser	(_registerDate, _userNo,	_endDate,		_blockCode,	_operationMemo)
										VALUES	(PaGamePublic.ufnGetCustomDatetime(),		@userNo,	@blockEndTime,	@blockCode,	@blockMemo)	
			IF(1 <> @@ROWCOUNT) -- 인서트중 오류
			 BEGIN
				SET @rv		= -4
				--SET @symNo	= 'eErrNoDbInternalError'
				GOTO LABEL_END	
			 END		
		 END

	 END	

	 --히스토리를 보관하기 위해 항상 모든 기록을 넣는다.
	 INSERT INTO PaGamePrivate.TblBlockHistory (_registerDate,	_userNo, _endDate,		_blockCode,	_operationMemo)
										VALUES (PaGamePublic.ufnGetCustomDatetime(),		@userNo, @blockEndTime,	@blockCode,	@blockMemo)
	IF(1 <> @@ROWCOUNT)
		 BEGIN
			SET @rv		= -5
			--SET @symNo	= 'eErrNoDbInternalError'
			GOTO LABEL_END	
		 END
              
	/**********************************************************************************************
	** 작업종료(이상에만기술)
	**********************************************************************************************/
LABEL_END:	
	IF(0 = @rv)
	 BEGIN
		COMMIT TRAN
	 END
	ELSE
	 BEGIN
		ROLLBACK TRAN
	 END	
	SELECT @rv
END
GO


-- ----------------------------
-- procedure structure for uspListBlockedUserByUserIdForDaum
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[uspListBlockedUserByUserIdForDaum]') AND type IN ('P', 'PC', 'RF', 'X'))
	DROP PROCEDURE[PaWebPublic].[uspListBlockedUserByUserIdForDaum]
GO

CREATE PROCEDURE [PaWebPublic].[uspListBlockedUserByUserIdForDaum]
	  @userId		NVARCHAR(30)
	--,@symNo			VARCHAR(50)     OUTPUT			-- 실패한경우에만의미있음(GameCommonSymNo.h에기술되어야함)
	-- 다음 웹에서 sqlrelay를 통해서 호출하는데 return 은 레코드셋으로만 받을 수 있고 output 파라미터로 받을 수 없어서 삭제
AS
/*!
 *  \brief		운영툴 : 유저Id로 유저의 블록 정보를 가져온다(다음 웹)
 *  \author		황지현
 */
BEGIN
	SET NOCOUNT ON										-- Count-set결과를생성하지말아라.
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	SET LOCK_TIMEOUT 3000								-- 함부로변경금지, 변경필요시DBA와협의.
	SET XACT_ABORT ON
             
	DECLARE @rv		INT = 0								-- 0:성공, 음수:오류코드, 양수:사용하면안됨(ODBC오류로처리함)

	/**********************************************************************************************
	** 작업시작(이하에만기술)
	** 코딩 규정 : JOIN 구문을 사용할 경우 반드시 별칭을 사용하며 컬럼명앞에 해당 별칭을 기술합니다
	**********************************************************************************************/
	--BEGIN TRAN	 uspListBlockedUserForDaum_Tx1	-- 트랜잭션 이름과 순서를 명시한다
	
	DECLARE @userNo	BIGINT                             
	SELECT @userNo = _userNo 
		FROM PaGamePrivate.TblUserInformation		 
		WHERE _userId = @userId
	IF(1 <> @@ROWCOUNT)
		 BEGIN
			SET @rv		= -1
			--SET @symNo	= 'eErrNoDbInternalError'
			GOTO LABEL_END	
		 END
			
	SELECT _blockCode, _registerDate, _endDate, _operationMemo 
		FROM PaGamePrivate.TblBlockHistory
		WHERE _userNo = @userNo
              
	/**********************************************************************************************
	** 작업종료(이상에만기술)
	**********************************************************************************************/
LABEL_END:	
	--IF(0 = @rv)
	-- BEGIN
	--	COMMIT TRAN
	-- END
	--ELSE
	-- BEGIN
	--	ROLLBACK TRAN
	-- END	
	--RETURN(@rv)
END
GO


-- ----------------------------
-- procedure structure for uspGetBlockedUserByUserIdForDaum
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[uspGetBlockedUserByUserIdForDaum]') AND type IN ('P', 'PC', 'RF', 'X'))
	DROP PROCEDURE[PaWebPublic].[uspGetBlockedUserByUserIdForDaum]
GO

CREATE PROCEDURE [PaWebPublic].[uspGetBlockedUserByUserIdForDaum]
	  @userId		NVARCHAR(30)
	--,@symNo			VARCHAR(50)     OUTPUT			-- 실패한경우에만의미있음(GameCommonSymNo.h에기술되어야함)
	-- 다음 웹에서 sqlrelay를 통해서 호출하는데 return 은 레코드셋으로만 받을 수 있고 output 파라미터로 받을 수 없어서 삭제
AS
/*!
 *  \brief		운영툴 : 유저Id로 유저의 블록 정보를 가져온다(다음 웹)
 *  \author		황지현
 */
BEGIN
	SET NOCOUNT ON										-- Count-set결과를생성하지말아라.
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	SET LOCK_TIMEOUT 3000								-- 함부로변경금지, 변경필요시DBA와협의.
	SET XACT_ABORT ON
             
	DECLARE @rv		INT = 0								-- 0:성공, 음수:오류코드, 양수:사용하면안됨(ODBC오류로처리함)

	/**********************************************************************************************
	** 작업시작(이하에만기술)
	** 코딩 규정 : JOIN 구문을 사용할 경우 반드시 별칭을 사용하며 컬럼명앞에 해당 별칭을 기술합니다
	**********************************************************************************************/
	--BEGIN TRAN	 uspGetBlockedUserForDaum_Tx1	-- 트랜잭션 이름과 순서를 명시한다
	
	DECLARE @currentTime	DATETIME	= PaGamePublic.ufnGetCustomDatetime()
	DECLARE @userNo			BIGINT                             
	SELECT @userNo = _userNo 
		FROM PaGamePrivate.TblUserInformation		 
		WHERE _userId = @userId
	IF(1 <> @@ROWCOUNT)
		 BEGIN
			SET @rv		= -1
			--SET @symNo	= 'eErrNoDbInternalError'
			GOTO LABEL_END	
		 END
			
	SELECT _blockCode, _registerDate, _endDate, _operationMemo
		FROM PaGamePrivate.TblBlockedUser
		WHERE (_userNo = @userNo) AND (@currentTime BETWEEN _registerDate AND _endDate)
              
	/**********************************************************************************************
	** 작업종료(이상에만기술)
	**********************************************************************************************/
LABEL_END:	
	--IF(0 = @rv)
	-- BEGIN
	--	COMMIT TRAN
	-- END
	--ELSE
	-- BEGIN
	--	ROLLBACK TRAN
	-- END	
	--RETURN(@rv)
END
GO


-- ----------------------------
-- procedure structure for uspBlockIPForDaum
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[uspBlockIPForDaum]') AND type IN ('P', 'PC', 'RF', 'X'))
	DROP PROCEDURE[PaWebPublic].[uspBlockIPForDaum]
GO

CREATE PROCEDURE [PaWebPublic].[uspBlockIPForDaum]
	 @startIP		VARCHAR(50)
	,@endIP			VARCHAR(50)	
	,@blockCode		INT
	,@blockMemo		NVARCHAR(200)
	--,@symNo			VARCHAR(50)     OUTPUT			-- 실패한경우에만의미있음(GameCommonSymNo.h에기술되어야함)
	-- 다음 웹에서 sqlrelay를 통해서 호출하는데 return 은 레코드셋으로만 받을 수 있고 output 파라미터로 받을 수 없어서 삭제
AS
/*!
 *  \brief		다음웹 : IP로 해당 범위를 블럭한다.
 *  \author		황지현
 */
BEGIN
	SET NOCOUNT ON										-- Count-set결과를생성하지말아라.
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	SET LOCK_TIMEOUT 3000								-- 함부로변경금지, 변경필요시DBA와협의.
	SET XACT_ABORT ON
             
	DECLARE @rv		INT = 0								-- 0:성공, 음수:오류코드, 양수:사용하면안됨(ODBC오류로처리함)

	/**********************************************************************************************
	** 작업시작(이하에만기술)
	** 코딩 규정 : JOIN 구문을 사용할 경우 반드시 별칭을 사용하며 컬럼명앞에 해당 별칭을 기술합니다
	**********************************************************************************************/
	BEGIN TRAN	 uspBlockIPForDaum_Tx1	-- 트랜잭션 이름과 순서를 명시한다
	
	DECLARE @bigintStartIP BIGINT =  PaGamePublic.ufnConvertDotIpToBigint( @startIP )
	DECLARE @bigintEndIP BIGINT =  PaGamePublic.ufnConvertDotIpToBigint( @endIP )
	
	-- IP와 기존 등록된 블럭 범위 안에 지금 등록하고자 하는 범위의 시작이나 종료가 포함되면 오류~
	IF EXISTS( SELECT * FROM PaGamePrivate.TblBlockedIP 
				WHERE (@bigintStartIP BETWEEN _bigintStartIP AND _bigintEndIP) OR (@bigintEndIP BETWEEN _bigintEndIP AND _bigintEndIP) )
	 BEGIN
		SET @rv		= -1
		--SET @symNo	= 'eErrNoDbInternalError'
		GOTO LABEL_END	
	 END
	ELSE -- 부분 포함 안되면 동일한 기록이 있다면 업데이트
	 BEGIN
		UPDATE PaGamePrivate.TblBlockedIP
		SET	_registerDate	= PaGamePublic.ufnGetCustomDatetime()
			,_startIP		= @startIP
			,_bigintStartIP	= @bigintStartIP
			,_endIP			= @endIP
			,_bigintEndIP	= @bigintEndIP
			,_blockCode		= @blockCode
			,_operationMemo	= @blockMemo
		WHERE (_bigintStartIP = @bigintStartIP) AND (_bigintEndIP = @bigintEndIP)
		IF(0 = @@ROWCOUNT)
		 BEGIN -- 포함도 아니고 동일한것도 없으면 인서트
			INSERT PaGamePrivate.TblBlockedIP	(_registerDate, _startIP,	_bigintStartIP,	_endIP, _bigintEndIP,	_blockCode,	_operationMemo)
										VALUES	(PaGamePublic.ufnGetCustomDatetime(),		@startIP,	@bigintStartIP,	@endIp,	@bigintEndIP,	@blockCode,	@blockMemo)	
			IF(1 <> @@ROWCOUNT)
				BEGIN
				SET @rv		= -2
				--SET @symNo	= 'eErrNoDbInternalError'
				GOTO LABEL_END	
				END
		 END
	 END

	 --히스토리를 보관하기 위해 항상 모든 기록을 넣는다.
	 INSERT INTO PaGamePrivate.TblIPBlockHistory(_registerDate, _startIP,	_endIP,	_blockCode,	_operationMemo)
										VALUES	(PaGamePublic.ufnGetCustomDatetime(),		@startIP,	@endIp,	@blockCode,	@blockMemo)
	IF(1 <> @@ROWCOUNT)
		 BEGIN
			SET @rv		= -3
			--SET @symNo	= 'eErrNoDbInternalError'
			GOTO LABEL_END	
		 END
              
	/**********************************************************************************************
	** 작업종료(이상에만기술)
	**********************************************************************************************/
LABEL_END:	
	IF(0 = @rv)
	 BEGIN
		COMMIT TRAN
	 END
	ELSE
	 BEGIN
		ROLLBACK TRAN
	 END	
	SELECT @rv
END
GO


-- ----------------------------
-- procedure structure for uspDeleteBlockedIPForDaum
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[uspDeleteBlockedIPForDaum]') AND type IN ('P', 'PC', 'RF', 'X'))
	DROP PROCEDURE[PaWebPublic].[uspDeleteBlockedIPForDaum]
GO

CREATE PROCEDURE [PaWebPublic].[uspDeleteBlockedIPForDaum]
	 @startIP		VARCHAR(50)
	,@endIP			VARCHAR(50)	
	,@blockCode		INT
	,@blockMemo		NVARCHAR(200)
	--,@symNo			VARCHAR(50)     OUTPUT			-- 실패한경우에만의미있음(GameCommonSymNo.h에기술되어야함)
	-- 다음 웹에서 sqlrelay를 통해서 호출하는데 return 은 레코드셋으로만 받을 수 있고 output 파라미터로 받을 수 없어서 삭제
AS
/*!
 *  \brief		다음웹 : IP로 해당 범위를 블럭 해제한다.
 *  \author		황지현
 */
BEGIN
	SET NOCOUNT ON										-- Count-set결과를생성하지말아라.
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	SET LOCK_TIMEOUT 3000								-- 함부로변경금지, 변경필요시DBA와협의.
	SET XACT_ABORT ON
             
	DECLARE @rv		INT = 0								-- 0:성공, 음수:오류코드, 양수:사용하면안됨(ODBC오류로처리함)

	/**********************************************************************************************
	** 작업시작(이하에만기술)
	** 코딩 규정 : JOIN 구문을 사용할 경우 반드시 별칭을 사용하며 컬럼명앞에 해당 별칭을 기술합니다
	**********************************************************************************************/
	BEGIN TRAN	 uspDeleteBlockedIPForDaum_Tx1	-- 트랜잭션 이름과 순서를 명시한다

	IF( @blockCode = 1600 ) --IP차단해제, 범위가 완전히 일치할때만 삭제한다
	 BEGIN
		DELETE PaGamePrivate.TblBlockedIP WHERE (_startIP = @startIP) AND (_endIP = @endIP)
		IF(1 <> @@ROWCOUNT)
		 BEGIN
			SET @rv		= -1
			--SET @symNo	= 'eErrNoDbInternalError'
			GOTO LABEL_END	
		 END
	 END
		 
	 --히스토리를 보관하기 위해 항상 모든 기록을 넣는다.
	 INSERT INTO PaGamePrivate.TblIPBlockHistory(_registerDate, _startIP,	_endIP,	_blockCode,	_operationMemo)
										VALUES	(PaGamePublic.ufnGetCustomDatetime(),		@startIP,	@endIp,	@blockCode,	@blockMemo)
	IF(1 <> @@ROWCOUNT)
		 BEGIN
			SET @rv		= -2
			--SET @symNo	= 'eErrNoDbInternalError'
			GOTO LABEL_END	
		 END
              
	/**********************************************************************************************
	** 작업종료(이상에만기술)
	**********************************************************************************************/
LABEL_END:	
	IF(0 = @rv)
	 BEGIN
		COMMIT TRAN
	 END
	ELSE
	 BEGIN
		ROLLBACK TRAN
	 END	
	SELECT @rv
END
GO


-- ----------------------------
-- procedure structure for uspInitPassword
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[uspInitPassword]') AND type IN ('P', 'PC', 'RF', 'X'))
	DROP PROCEDURE[PaWebPublic].[uspInitPassword]
GO

CREATE PROCEDURE [PaWebPublic].[uspInitPassword]
	 @userId	nvarchar(30)
	--,@symNo					NVARCHAR(50)     OUTPUT             -- 실패한경우에만의미있음(GameCommonSymNo.h에기술되어야함)
AS
/*!
 *  \brief		_password NULL 초기화
 *  \author		차주언
 */
BEGIN
	SET NOCOUNT ON										-- Count-set결과를생성하지말아라.
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	SET LOCK_TIMEOUT 3000								-- 함부로변경금지, 변경필요시DBA와협의.
	SET XACT_ABORT ON
             
	DECLARE @rv		INT = 1								-- 다음 웹에서 호출시 성공을 1로 리턴해야한다

	/**********************************************************************************************
	** 작업시작(이하에만기술)
	** 코딩 규정 : JOIN 구문을 사용할 경우 반드시 별칭을 사용하며 컬럼명앞에 해당 별칭을 기술합니다
	**********************************************************************************************/
	DECLARE @ERROR BIGINT,@ROWCOUNT BIGINT

	UPDATE PaGamePrivate.TblUserInformation
	SET	 _password			= NULL
		,_paymentPassword	= NULL
		,_failPasswordCount	= 0
	WHERE _userId = @userId

	SELECT @ERROR = @@ERROR, @ROWCOUNT = @@ROWCOUNT

	IF(@ROWCOUNT <> 1)
	 BEGIN
		SET @rv		= -1
		--SET @symNo	=	N'eErrNoUserIdIsInvalid'
		GOTO LABEL_END	
	 END

	IF (@ERROR <> 0)
	 BEGIN
		SET @rv         = -2
		--SET @symNo	=	N'eErrNoDbInternalError'
		GOTO LABEL_END                        
	 END
              
	/**********************************************************************************************
	** 작업종료(이상에만기술)
	**********************************************************************************************/
LABEL_END:
	/*
	IF(0 = @rv)
		COMMIT TRAN
	ELSE
		ROLLBACK TRAN
	*/
	SELECT @rv
END
GO


-- ----------------------------
-- procedure structure for uspIsGameMaster
-- ----------------------------
IF EXISTS (SELECT * FROM sys.all_objects WHERE object_id = OBJECT_ID(N'[PaWebPublic].[uspIsGameMaster]') AND type IN ('P', 'PC', 'RF', 'X'))
	DROP PROCEDURE[PaWebPublic].[uspIsGameMaster]
GO

CREATE PROCEDURE [PaWebPublic].[uspIsGameMaster]
    @userNo    BIGINT   
   ,@isGameMaster         BIT OUTPUT     
   ,@symNo        NVARCHAR(MAX)   OUTPUT
 	 ,@rv  INT   OUTPUT
AS

    
    SET @rv = 0
    SET @symNo = N''
GO


-- ----------------------------
-- Primary Key structure for table link
-- ----------------------------
ALTER TABLE [PaWebPublic].[link] ADD CONSTRAINT [PK__link__3213E83F84DF3C8C] PRIMARY KEY CLUSTERED ([id])
WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON)  
ON [PRIMARY]
GO


-- ----------------------------
-- Primary Key structure for table news
-- ----------------------------
ALTER TABLE [PaWebPublic].[news] ADD CONSTRAINT [PK__news__3213E83FB4050558] PRIMARY KEY CLUSTERED ([id])
WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON)  
ON [PRIMARY]
GO


-- ----------------------------
-- Primary Key structure for table shop
-- ----------------------------
ALTER TABLE [PaWebPublic].[shop] ADD CONSTRAINT [PK__shop__3213E83FF473418E] PRIMARY KEY CLUSTERED ([id])
WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON)  
ON [PRIMARY]
GO


-- ----------------------------
-- Primary Key structure for table slide
-- ----------------------------
ALTER TABLE [PaWebPublic].[slide] ADD CONSTRAINT [PK__slide__3213E83F2AF2D859] PRIMARY KEY CLUSTERED ([id])
WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON)  
ON [PRIMARY]
GO


-- ----------------------------
-- Primary Key structure for table version
-- ----------------------------
ALTER TABLE [PaWebPublic].[version] ADD CONSTRAINT [PK__version__3213E83FFE2A9B95] PRIMARY KEY CLUSTERED ([id])
WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON)  
ON [PRIMARY]
GO

