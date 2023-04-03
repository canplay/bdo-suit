<template>
  <q-table
    :rows="table.user.rows"
    :columns="table.user.columns"
    row-key="userNo"
    separator="cell"
    v-model:pagination="table.user.pagination"
    :filter="table.user.filter"
    @request="onRequest"
    selection="multiple"
    v-model:selected="table.user.selected"
  >
    <template v-slot:top>
      <div class="fit row">
        <div class="col text-h5 flex items-center">用户管理</div>

        <q-space class="col-auto" />

        <q-input filled v-model="table.user.search" label="用户名" />

        <div class="col-auto" style="width: 8px" />

        <q-btn
          class="col-1"
          color="secondary"
          label="发送邮件"
          :disable="table.user.selected.length === 0"
          @click="dialog.mail.show = true"
        />

        <div class="col-auto" style="width: 8px" />

        <q-btn class="col-1" color="primary" label="查询" @click="onQuery" />
      </div>
    </template>

    <template v-slot:header-cell="props">
      <q-th :props="props" style="font: bold 15px arial, sans-serif">
        {{ props.col.label }}
      </q-th>
    </template>

    <template v-slot:body-selection="props">
      <q-checkbox v-model="props.selected" />
    </template>

    <template v-slot:body-cell="props">
      <q-td v-if="props.col.name === 'userNo'" :props="props">
        <q-btn
          color="primary"
          :label="props.value"
          @click="onView(props.row)"
        />
      </q-td>

      <q-td v-else-if="props.col.name === 'characters'" :props="props">
        <q-list bordered v-for="item in props.value" :key="item">
          <q-item class="text-left">
            <q-item-section>
              <q-item-label overline>
                角色名：{{ item.characterName }}
              </q-item-label>

              <q-item-label overline> 职业：{{ item.classType }} </q-item-label>

              <q-item-label caption>
                负重：{{ item.variedWeight / 100000 }}
              </q-item-label>

              <q-item-label caption>
                游玩时长：{{ item.totalPlayTime }}
              </q-item-label>
            </q-item-section>

            <q-item-section side top>
              <q-item-label caption> 等级：{{ item.level }} </q-item-label>

              <q-item-label caption>
                攻击力：{{ item.offenceValue }}
              </q-item-label>

              <q-item-label caption>
                防御力：{{ item.defenceValue }}
              </q-item-label>

              <q-item-label caption>
                觉醒攻击力：{{ item.awakenValue }}
              </q-item-label>
            </q-item-section>
          </q-item>
        </q-list>
      </q-td>

      <q-td v-else :props="props">
        <div
          style="
            max-width: 400px;
            overflow: hidden;
            text-overflow: ellipsis;
            white-space: nowrap;
          "
        >
          {{ props.value }}
        </div>
      </q-td>
    </template>
  </q-table>

  <div style="height: 8px" />

  <q-table
    :rows="table.gm.rows"
    :columns="table.gm.columns"
    row-key="id"
    separator="cell"
    v-model:pagination="table.gm.pagination"
    :filter="table.gm.filter"
    @request="onGmRequest"
    selection="multiple"
    v-model:selected="table.gm.selected"
  >
    <template v-slot:top>
      <div class="fit row">
        <div class="col text-h5 flex items-center">GM管理</div>

        <q-space class="col-auto" />

        <q-btn
          :disable="table.gm.selected.length === 0"
          class="col-1"
          color="negative"
          label="删除"
          @click="onGmDel"
        />

        <div class="col-auto" style="width: 8px" />

        <q-btn class="col-1" color="secondary" label="添加" @click="onGmAdd" />

        <div class="col-auto" style="width: 8px" />

        <q-btn class="col-1" color="primary" label="查询" @click="onGmQuery" />
      </div>
    </template>

    <template v-slot:header-cell="props">
      <q-th :props="props" style="font: bold 15px arial, sans-serif">
        {{ props.col.label }}
      </q-th>
    </template>

    <template v-slot:body-selection="props">
      <q-checkbox v-model="props.selected" />
    </template>
  </q-table>

  <q-dialog v-model="dialog.user.show">
    <q-card style="min-width: 80vw">
      <q-card-section class="text-h6">编辑用户</q-card-section>

      <q-card-section>
        <div class="row">
          <div class="col text-h6 column">
            <div class="col">编号: {{ dialog.user.userNo }}</div>
            <div class="col">ID: {{ dialog.user.userId }}</div>
            <div class="col">创建时间: {{ dialog.user.registerDate }}</div>
          </div>

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col-2"
            v-model="dialog.user.username"
            label="用户名"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input class="col-2" v-model="dialog.user.password" label="密码" />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col-2"
            v-model="dialog.user.userNickname"
            label="角色名"
          />
        </div>

        <div style="height: 8px" />

        <div class="row">
          <div class="col text-h6 column">
            <div class="col">游玩时长: {{ dialog.user.totalPlayTime }}</div>
            <div class="col">最近上线时间: {{ dialog.user.lastLoginTime }}</div>
            <div class="col">
              最近下线时间: {{ dialog.user.lastLogoutTime }}
            </div>
          </div>

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col-2"
            v-model="dialog.user.pcroom"
            label="是否网吧加成"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col-2"
            v-model="dialog.user.membershipType"
            label="会员等级"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col-2"
            v-model="dialog.user.isValid"
            label="是否启用"
          />
        </div>

        <div style="height: 8px" />

        <q-table
          class="my-sticky-column-table"
          :rows="dialog.user.characters.rows"
          :columns="dialog.user.characters.columns"
          row-key="characterNo"
          separator="cell"
          v-model:pagination="dialog.user.characters.pagination"
          :filter="dialog.user.characters.filter"
        >
          <template v-slot:header-cell="props">
            <q-th :props="props" style="font: bold 15px arial, sans-serif">
              {{ props.col.label }}
            </q-th>
          </template>

          <template v-slot:body-cell="props">
            <q-td v-if="props.col.name === 'characterNo'" :props="props">
              <div
                style="
                  max-width: 400px;
                  overflow: hidden;
                  text-overflow: ellipsis;
                  white-space: nowrap;
                "
              >
                {{ props.value }}
              </div>
            </q-td>

            <q-td v-else-if="props.col.name === 'deletedDate'" :props="props">
              {{ props.value === '' ? 0 : 1 }}
              <q-popup-edit v-model="props.value" auto-save v-slot="scope">
                <q-input
                  v-model="scope.value"
                  dense
                  autofocus
                  counter
                  @keyup.enter="scope.set"
                />
              </q-popup-edit>
            </q-td>

            <q-td v-else :props="props">
              <div
                style="
                  max-width: 400px;
                  overflow: hidden;
                  text-overflow: ellipsis;
                  white-space: nowrap;
                "
              >
                {{ props.value }}
                <q-popup-edit v-model="props.value" auto-save v-slot="scope">
                  <q-input
                    v-model="scope.value"
                    dense
                    autofocus
                    counter
                    @keyup.enter="scope.set"
                  />
                </q-popup-edit>
              </div>
            </q-td>
          </template>
        </q-table>
      </q-card-section>

      <q-card-actions class="row">
        <q-btn class="col" label="保存" color="primary" @click="onSave()" />
      </q-card-actions>
    </q-card>
  </q-dialog>

  <q-dialog v-model="dialog.mail.show">
    <q-card style="min-width: 80vw">
      <q-card-section class="text-h6">编辑邮件</q-card-section>

      <q-card-section>
        <div class="row">
          <q-input
            class="col"
            v-model="dialog.mail.senderName"
            label="发件人名称(可选)"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.mail.senderUserNo"
            label="发件人编号(可选)"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.mail.receiverName"
            label="收件人名称(可选)"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.mail.receiverUserNo"
            label="收件人编号(可选)"
          />
        </div>

        <div style="height: 8px" />

        <div class="row">
          <q-input class="col" v-model="dialog.mail.title" label="标题" />

          <div class="col-auto" style="width: 8px" />

          <q-input class="col" v-model="dialog.mail.contents" label="内容" />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.mail.mailType"
            label="邮件类型"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input class="col" v-model="dialog.mail.variousNo" label="物品ID">
            <template v-slot:append>
              <q-btn
                color="primary"
                label="ID列表"
                @click="store.dialog.items = true"
              />
            </template>
          </q-input>
        </div>

        <div style="height: 8px" />

        <div class="row">
          <q-input
            class="col"
            v-model="dialog.mail.enchantLevel"
            label="强化等级"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.mail.itemCount"
            label="物品数量"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.mail.webItemType"
            label="Web物品类型"
          />
        </div>
      </q-card-section>

      <q-card-actions class="row">
        <q-btn class="col" label="发送" color="primary" @click="onSendMail()" />
      </q-card-actions>
    </q-card>
  </q-dialog>

  <q-dialog v-model="dialog.gm.show">
    <q-card style="min-width: 80vw">
      <q-card-section class="text-h6">添加GM</q-card-section>

      <q-card-section class="row">
        <q-input class="col" v-model="dialog.gm.id" label="编号" />

        <div class="col-auto" style="width: 8px" />

        <q-select
          class="col"
          v-model="dialog.gm.level.model"
          :options="dialog.gm.level.options"
          label="等级"
        />
      </q-card-section>

      <q-card-actions class="row">
        <q-btn class="col" label="保存" color="primary" @click="onGmSave()" />
      </q-card-actions>
    </q-card>
  </q-dialog>

  <itemComp />
</template>

<script setup lang="ts">
import { ref } from 'vue';
import { useQuasar, QTableProps } from 'quasar';
import { useStore } from 'stores/store';
import useFetch from 'src/components/fetch';
import itemComp from 'src/components/itemComp.vue';

const $q = useQuasar();
const store = useStore();

const table = ref({
  user: {
    pagination: {
      page: 1,
      rowsNumber: 0,
      rowsPerPage: 7,
      sortBy: '',
      descending: true,
    } as QTableProps['pagination'],
    filter: '',
    search: '',
    selected: ref([] as any),
    columns: [
      {
        name: 'userNo',
        label: '编号',
        field: 'userNo',
        align: 'center',
        sortable: true,
      },
      {
        name: 'userId',
        label: 'ID',
        field: 'userId',
        align: 'center',
        sortable: true,
      },
      {
        name: 'userNickname',
        label: '家族名',
        field: 'userNickname',
        align: 'center',
        sortable: true,
      },
      {
        name: 'lastLoginTime',
        label: '最近上线时间',
        field: 'lastLoginTime',
        align: 'center',
        sortable: true,
      },
      {
        name: 'lastLogoutTime',
        label: '最近下线时间',
        field: 'lastLogoutTime',
        align: 'center',
        sortable: true,
      },
      {
        name: 'totalPlayTime',
        label: '游玩时间',
        field: 'totalPlayTime',
        align: 'center',
        sortable: true,
      },
      {
        name: 'membershipType',
        label: '会员类型',
        field: 'membershipType',
        align: 'center',
        sortable: true,
      },
      {
        name: 'pcroom',
        label: '是否网吧加成',
        field: 'pcroom',
        align: 'center',
        sortable: true,
      },
      {
        name: 'valid',
        label: '是否启用',
        field: 'valid',
        align: 'center',
        sortable: true,
      },
      {
        name: 'characters',
        label: '游戏角色',
        field: 'characters',
        align: 'center',
        sortable: true,
      },
    ] as QTableProps['columns'],
    rows: [] as any,
  },
  gm: {
    pagination: {
      page: 1,
      rowsNumber: 0,
      rowsPerPage: 7,
      sortBy: '',
      descending: true,
    } as QTableProps['pagination'],
    filter: '',
    selected: ref([] as any),
    columns: [
      {
        name: 'id',
        label: '编号',
        field: 'id',
        align: 'center',
        sortable: true,
      },
      {
        name: 'userNo',
        label: 'ID',
        field: 'userNo',
        align: 'center',
        sortable: true,
      },
      {
        name: 'permission',
        label: '权限',
        field: 'permission',
        align: 'center',
        sortable: true,
      },
      {
        name: 'delete',
        label: '是否删除',
        field: 'delete',
        align: 'center',
        sortable: true,
      },
      {
        name: 'create_user',
        label: '创建人',
        field: 'create_user',
        align: 'center',
        sortable: true,
      },
      {
        name: 'create_date',
        label: '创建时间',
        field: 'create_date',
        align: 'center',
        sortable: true,
      },
      {
        name: 'update_user',
        label: '更新人',
        field: 'update_user',
        align: 'center',
        sortable: true,
      },
      {
        name: 'update_date',
        label: '更新时间',
        field: 'update_date',
        align: 'center',
        sortable: true,
      },
    ] as QTableProps['columns'],
    rows: [] as any,
  },
});

const dialog = ref({
  user: {
    show: false,
    userId: '',
    username: '',
    password: '',
    userNo: 0,
    userNickname: '',
    totalPlayTime: 0,
    isValid: 1,
    pcroom: 0,
    membershipType: 0,
    registerDate: '',
    lastLoginTime: '',
    lastLogoutTime: '',
    characters: {
      pagination: {
        page: 1,
        rowsNumber: 0,
        rowsPerPage: 7,
        sortBy: '',
        descending: true,
      } as QTableProps['pagination'],
      filter: '',
      columns: [
        {
          name: 'characterNo',
          label: '编号',
          field: 'characterNo',
          align: 'center',
          sortable: true,
        },
        {
          name: 'characterName',
          label: '角色名',
          field: 'characterName',
          align: 'center',
          sortable: true,
        },
        {
          name: 'classType',
          label: '职业',
          field: 'classType',
          align: 'center',
          sortable: true,
        },
        {
          name: 'totalPlayTime',
          label: '游玩时长',
          field: 'totalPlayTime',
          align: 'center',
          sortable: true,
        },
        {
          name: 'level',
          label: '等级',
          field: 'level',
          align: 'center',
          sortable: true,
        },
        {
          name: 'experience',
          label: '经验值',
          field: 'experience',
          align: 'center',
          sortable: true,
        },
        {
          name: 'skillPointLevel',
          label: '技能点等级',
          field: 'skillPointLevel',
          align: 'center',
          sortable: true,
        },
        {
          name: 'skillPointExperience',
          label: '技能点经验值',
          field: 'skillPointExperience',
          align: 'center',
          sortable: true,
        },
        {
          name: 'remainedSkillPoint',
          label: '剩余技能点',
          field: 'remainedSkillPoint',
          align: 'center',
          sortable: true,
        },
        {
          name: 'aquiredSkillPoint',
          label: '获得的技能点',
          field: 'aquiredSkillPoint',
          align: 'center',
          sortable: true,
        },
        {
          name: 'tendency',
          label: '善恶倾向',
          field: 'tendency',
          align: 'center',
          sortable: true,
        },
        {
          name: 'variedWeight',
          label: '负重',
          field: 'variedWeight',
          align: 'center',
          sortable: true,
        },
        {
          name: 'inventorySlotCount',
          label: '背包数量',
          field: 'inventorySlotCount',
          align: 'center',
          sortable: true,
        },
        {
          name: 'titleKey',
          label: '头衔',
          field: 'titleKey',
          align: 'center',
          sortable: true,
        },
        {
          name: 'killRewardCount',
          label: '击杀奖励数',
          field: 'killRewardCount',
          align: 'center',
          sortable: true,
        },
        {
          name: 'enchantFailCount',
          label: '强化失败数',
          field: 'enchantFailCount',
          align: 'center',
          sortable: true,
        },
        {
          name: 'currentPositionX',
          label: '当前坐标X',
          field: 'currentPositionX',
          align: 'center',
          sortable: true,
        },
        {
          name: 'currentPositionY',
          label: '当前坐标Y',
          field: 'currentPositionY',
          align: 'center',
          sortable: true,
        },
        {
          name: 'currentPositionZ',
          label: '当前坐标Z',
          field: 'currentPositionZ',
          align: 'center',
          sortable: true,
        },
        {
          name: 'hp',
          label: 'HP',
          field: 'hp',
          align: 'center',
          sortable: true,
        },
        {
          name: 'mp',
          label: 'MP',
          field: 'mp',
          align: 'center',
          sortable: true,
        },
        {
          name: 'sp',
          label: 'SP',
          field: 'sp',
          align: 'center',
          sortable: true,
        },
        {
          name: 'wp',
          label: 'WP',
          field: 'wp',
          align: 'center',
          sortable: true,
        },
        {
          name: 'defenceValue',
          label: '防御力',
          field: 'defenceValue',
          align: 'center',
          sortable: true,
        },
        {
          name: 'offenceValue',
          label: '攻击力',
          field: 'offenceValue',
          align: 'center',
          sortable: true,
        },
        {
          name: 'awakenValue',
          label: '觉醒攻击力',
          field: 'awakenValue',
          align: 'center',
          sortable: true,
        },
        {
          name: 'deletedDate',
          label: '是否删除',
          field: 'deletedDate',
          align: 'center',
          sortable: true,
        },
      ] as QTableProps['columns'],
      rows: [] as any,
    },
  },
  mail: {
    show: false,
    senderName: 'GM',
    senderUserNo: '0',
    receiverName: '',
    receiverUserNo: '-1',
    title: 'GM邮件',
    contents: 'GM邮件内容',
    mailType: '0',
    variousNo: '-1',
    enchantLevel: '0',
    itemCount: '0',
    webItemType: '0',
  },
  gm: {
    show: false,
    id: '',
    level: {
      model: { label: '普通用户', value: '7752a77001b84cdb8c0581a3b115190b' },
      options: [
        { label: '普通用户', value: '7752a77001b84cdb8c0581a3b115190b' },
        { label: '管理员', value: '53a4d05b5f06f85e80ef548f509f2355' },
      ],
    },
  },
});

const onRequest = (props: any) => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  let { page, rowsPerPage, sortBy, descending, rowsNumber } = props.pagination;

  table.value.user.rows = [];
  dialog.value.user.characters.rows = [];

  useFetch()
    .post(
      store.backend + '/api/user/info',
      {
        curPage: (page - 1) * rowsPerPage,
        maxPage: rowsPerPage === 0 ? rowsNumber : rowsPerPage,
        sortBy: sortBy === '' ? 'userNo' : sortBy,
        descending: descending,
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      if (resp.data.status != 0) {
        for (let i = 0; i < resp.data.msg.length; ++i) {
          if (resp.data.msg[i].id === '') break;

          table.value.user.rows.push({
            registerDate: resp.data.msg[i].registerDate,
            isValid: resp.data.msg[i].valid,
            userNo: resp.data.msg[i].userNo,
            userId: resp.data.msg[i].userId,
            userNickname: resp.data.msg[i].userNickname,
            lastLoginTime: resp.data.msg[i].lastLoginTime.replace(' +0800', ''),
            lastLogoutTime: resp.data.msg[i].lastLogoutTime.replace(
              ' +0800',
              ''
            ),
            totalPlayTime: resp.data.msg[i].totalPlayTime,
            membershipType: resp.data.msg[i].membershipType,
            pcroom: resp.data.msg[i].pcroom,
            characters: resp.data.msg[i].characters,
          });
        }
      } else {
        $q.notify('网络错误，请稍后重试');
      }

      table.value.user.pagination!.page = page;
      table.value.user.pagination!.rowsPerPage = rowsPerPage;
      table.value.user.pagination!.sortBy = sortBy;
      table.value.user.pagination!.descending = descending;

      $q.loading.hide();
      clearTimeout(time);
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onQuery = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  useFetch()
    .get(store.backend + '/api/user/count', $q.cookies.get('canplay_token'))
    .then((resp) => {
      table.value.user.pagination!.rowsNumber = parseInt(resp.data.msg);
      onRequest({ pagination: table.value.user.pagination });

      $q.loading.hide();
      clearTimeout(time);
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onView = (val: any) => {
  dialog.value.user.show = true;
  dialog.value.user.userNo = val.userNo;
  dialog.value.user.userId = val.userId;
  dialog.value.user.userNickname = val.userNickname;
  dialog.value.user.totalPlayTime = val.totalPlayTime;
  dialog.value.user.isValid = val.isValid;
  dialog.value.user.pcroom = val.pcroom;
  dialog.value.user.membershipType = val.membershipType;
  dialog.value.user.registerDate = val.registerDate.replace(' +0800', '');
  dialog.value.user.lastLoginTime = val.lastLoginTime.replace(' +0800', '');
  dialog.value.user.lastLogoutTime = val.lastLogoutTime.replace(' +0800', '');
  dialog.value.user.username = val.userId.split(',')[0];
  dialog.value.user.password = val.userId.split(',')[1];

  if (val.characters != null) {
    for (let index = 0; index < val.characters.length; index++) {
      const element = val.characters[index];
      dialog.value.user.characters.rows.push(element);
    }
  }
};

const onSendMail = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  for (let index = 0; index < table.value.user.selected.length; index++) {
    const element = table.value.user.selected[index];

    useFetch()
      .post(
        store.backend + '/api/user/admin/mail',
        {
          senderName: dialog.value.mail.senderName,
          senderUserNo: parseInt(dialog.value.mail.senderUserNo),
          receiverName: element.userNickname,
          receiverUserNo: parseInt(element.userNo),
          title: dialog.value.mail.title,
          contents: dialog.value.mail.contents,
          mailType: parseInt(dialog.value.mail.mailType),
          variousNo: parseInt(dialog.value.mail.variousNo),
          enchantLevel: parseInt(dialog.value.mail.enchantLevel),
          itemCount: parseInt(dialog.value.mail.itemCount),
          webItemType: parseInt(dialog.value.mail.webItemType),
          expirationDate: '',
        },
        $q.cookies.get('canplay_token')
      )
      .then((resp) => {
        $q.loading.hide();
        clearTimeout(time);

        if (resp.data.status === 0) {
          $q.notify(resp.data.msg);
          return;
        }

        $q.notify('发送邮件成功');
      })
      .catch(() => {
        $q.notify('网络错误，请稍后重试');
        $q.loading.hide();
        clearTimeout(time);
      });
  }
};

const onSave = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  useFetch()
    .post(
      store.backend + '/api/user/admin/update',
      {
        userNo: dialog.value.user.userNo,
        isValid: dialog.value.user.isValid,
        username: dialog.value.user.username,
        password: dialog.value.user.password,
        userNickname: dialog.value.user.userNickname,
        pcroom: dialog.value.user.pcroom,
        membershipType: dialog.value.user.membershipType,
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      $q.loading.hide();
      clearTimeout(time);

      if (resp.data.status === 0) {
        $q.notify(resp.data.msg);
        return;
      }

      for (
        let index = 0;
        index < dialog.value.user.characters.rows.length;
        index++
      ) {
        const element = dialog.value.user.characters.rows[index];

        useFetch()
          .post(
            store.backend + '/api/user/admin/character/update',
            {
              characterName: element.characterName,
              currentPositionX: element.currentPositionX,
              currentPositionY: element.currentPositionY,
              currentPositionZ: element.currentPositionZ,
              level: element.level,
              experience: element.experience,
              skillPointLevel: element.skillPointLevel,
              skillPointExperience: element.skillPointExperience,
              remainedSkillPoint: element.remainedSkillPoint,
              aquiredSkillPoint: element.aquiredSkillPoint,
              tendency: element.tendency,
              variedWeight: element.variedWeight,
              hp: element.hp,
              mp: element.mp,
              sp: element.sp,
              wp: element.wp,
              inventorySlotCount: element.inventorySlotCount,
              titleKey: element.titleKey,
              killRewardCount: element.killRewardCount,
              enchantFailCount: element.enchantFailCount,
              offenceValue: element.offenceValue,
              defenceValue: element.defenceValue,
              awakenValue: element.awakenValue,
              characterNo: element.characterNo,
              deletedDate: element.deletedDate === 1 ? '1' : '',
            },
            $q.cookies.get('canplay_token')
          )
          .then((resp) => {
            $q.loading.hide();
            clearTimeout(time);

            if (resp.data.status === 0) {
              $q.notify(resp.data.msg);
              return;
            }

            $q.notify('更新账号成功');
          })
          .catch(() => {
            $q.notify('网络错误，请稍后重试');
          });
      }
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');

      $q.loading.hide();
      clearTimeout(time);
    });
};

const onGmRequest = (props: any) => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  let { page, rowsPerPage, sortBy, descending, rowsNumber } = props.pagination;

  table.value.gm.rows = [];

  useFetch()
    .post(
      store.backend + '/api/user/admin/gm/info/web',
      {
        curPage: (page - 1) * rowsPerPage,
        maxPage: rowsPerPage === 0 ? rowsNumber : rowsPerPage,
        sortBy: sortBy === '' ? 'userNo' : sortBy,
        descending: descending,
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      if (resp.data.status != 0) {
        for (let i = 0; i < resp.data.msg.length; ++i) {
          if (resp.data.msg[i].id === '') break;

          table.value.gm.rows.push({
            id: resp.data.msg[i].id,
            userNo: resp.data.msg[i].userno,
            permission: resp.data.msg[i].permission,
            create_user: resp.data.msg[i].create_user,
            create_date: resp.data.msg[i].create_date.replace(' +0800', ''),
            update_user: resp.data.msg[i].update_user,
            update_date: resp.data.msg[i].update_date.replace(' +0800', ''),
            delete: resp.data.msg[i].delete,
          });
        }
      } else {
        $q.notify('网络错误，请稍后重试');
      }

      table.value.gm.pagination!.page = page;
      table.value.gm.pagination!.rowsPerPage = rowsPerPage;
      table.value.gm.pagination!.sortBy = sortBy;
      table.value.gm.pagination!.descending = descending;

      $q.loading.hide();
      clearTimeout(time);
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onGmQuery = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  useFetch()
    .get(
      store.backend + '/api/user/admin/gm/count/web',
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      $q.loading.hide();
      clearTimeout(time);

      table.value.gm.pagination!.rowsNumber = parseInt(resp.data.msg);
      onGmRequest({ pagination: table.value.gm.pagination });
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onGmSave = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  useFetch()
    .put(
      store.backend + '/api/user/admin/gm/update/web',
      {
        userNo: parseInt(dialog.value.gm.id),
        permission: dialog.value.gm.level.model.value,
        create_user: store.user.userno.toString(),
        update_user: store.user.userno.toString(),
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      $q.loading.hide();
      clearTimeout(time);

      if (resp.data.status === 0) {
        $q.notify(resp.data.msg);
        return;
      }

      $q.notify('添加成功');
      dialog.value.gm.show = false;
      onGmQuery();
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onGmAdd = () => {
  dialog.value.gm.id = '';
  dialog.value.gm.show = true;
};

const onGmDel = (val: any) => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  for (let index = 0; index < table.value.gm.selected.length; index++) {
    const element = table.value.gm.selected[index];

    useFetch()
      .post(
        store.backend + '/api/user/admin/gm/update/web',
        {
          id: element.id,
        },
        $q.cookies.get('canplay_token')
      )
      .then((resp) => {
        $q.loading.hide();
        clearTimeout(time);

        if (resp.data.status === 0) {
          $q.notify(resp.data.msg);
          return;
        }

        $q.notify('删除成功');
        onGmQuery();
      })
      .catch(() => {
        $q.notify('网络错误，请稍后重试');
        $q.loading.hide();
        clearTimeout(time);
      });
  }
};
</script>
