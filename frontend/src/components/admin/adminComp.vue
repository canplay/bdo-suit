<template>
  <q-page>
    <q-table
      :rows="table.rows"
      :columns="table.columns"
      row-key="userNo"
      separator="cell"
      v-model:pagination="table.pagination"
      :filter="table.filter"
      @request="onRequest"
      selection="multiple"
      v-model:selected="table.selected"
    >
      <template v-slot:top>
        <div class="fit row">
          <div class="col text-h5 flex items-center">GM管理</div>

          <q-space class="col-auto" />

          <q-btn
            class="col-1"
            color="secondary"
            label="撤销GM"
            :disable="table.selected.length === 0"
            @click="onDeactive()"
          />

          <div class="col-auto" style="width: 8px" />

          <q-btn
            class="col-1"
            color="secondary"
            label="激活GM"
            :disable="table.selected.length === 0"
            @click="onActive()"
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
    </q-table>
  </q-page>

  <q-dialog v-model="dialog.show">
    <q-card style="min-width: 80vw">
      <q-card-section class="text-h6">编辑GM</q-card-section>

      <q-card-section> </q-card-section>

      <q-card-actions class="row">
        <q-btn class="col" label="删除" color="negative" @click="onDel()" />

        <div class="col-auto" style="width: 8px" />

        <q-btn class="col" label="保存" color="primary" @click="onSave()" />
      </q-card-actions>
    </q-card>
  </q-dialog>
</template>

<script setup lang="ts">
import { ref } from 'vue';
import { useQuasar, QTableProps } from 'quasar';
import { useStore } from 'stores/store';
import useFetch from 'src/components/fetch';

const $q = useQuasar();
const store = useStore();

const table = ref({
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
      label: 'ID',
      field: 'id',
      align: 'center',
      sortable: true,
    },
    {
      name: 'userno',
      label: '编号',
      field: 'userno',
      align: 'center',
      sortable: true,
    },
    {
      name: 'familyname',
      label: '家族名',
      field: 'familyname',
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
      name: 'create_user',
      label: '创建人',
      field: 'create_user',
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
    {
      name: 'update_user',
      label: '更新人',
      field: 'update_user',
      align: 'center',
      sortable: true,
    },
  ] as QTableProps['columns'],
  rows: [] as any,
});

const dialog = ref({
  show: false,
  id: '',
  userno: '',
  familyname: '',
  create_date: '',
  create_user: '',
  update_date: '',
  update_user: '',
});

const onRequest = (props: any) => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  let { page, rowsPerPage, sortBy, descending, rowsNumber } = props.pagination;

  table.value.rows = [];

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

          table.value.rows.push({
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

      table.value.pagination!.page = page;
      table.value.pagination!.rowsPerPage = rowsPerPage;
      table.value.pagination!.sortBy = sortBy;
      table.value.pagination!.descending = descending;

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
      table.value.pagination!.rowsNumber = parseInt(resp.data.msg);
      onRequest({ pagination: table.value.pagination });

      $q.loading.hide();
      clearTimeout(time);
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onActive = () => {
  return;
};

const onDeactive = () => {
  return;
};

const onSave = () => {
  return;
};
</script>
