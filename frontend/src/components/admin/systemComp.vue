<template>
  <q-table
    :rows="table.version.rows"
    :columns="table.version.columns"
    row-key="id"
    separator="cell"
    v-model:pagination="table.version.pagination"
    :filter="table.version.filter"
    @request="onRequest"
  >
    <template v-slot:top>
      <div class="fit row">
        <div class="col text-h5 flex items-center">版本管理</div>

        <q-space class="col-auto" />

        <q-btn class="col-2" color="primary" label="查询" @click="onQuery" />

        <div class="col-auto" style="width: 8px" />

        <q-btn
          class="col-2"
          color="red"
          label="新增"
          @click="
            dialog.version = {
              show: true,
              id: '',
              name: {
                model: '',
                options: ['app', 'resource'],
              },
              version: '',
              force: {
                model: { label: '否', value: 0 },
                options: [
                  { label: '否', value: 0 },
                  { label: '是', value: 1 },
                ],
              },
              link: '',
              dir: {
                oldPath: '',
                newPath: '',
                savePath: '',
              },
            }
          "
        />
      </div>
    </template>

    <template v-slot:header-cell="props">
      <q-th :props="props" style="font: bold 15px arial, sans-serif">
        {{ props.col.label }}
      </q-th>
    </template>

    <template v-slot:body-cell="props">
      <q-td v-if="props.col.name === 'id'" :props="props">
        <q-btn
          color="primary"
          :label="props.value"
          @click="onView(props.row)"
        />
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

  <q-dialog v-model="dialog.version.show">
    <q-card style="min-width: 80vw">
      <q-card-section class="text-h6">编辑版本</q-card-section>

      <q-card-section>
        <div class="row">
          <q-select
            class="col"
            v-model="dialog.version.name.model"
            :options="dialog.version.name.options"
            label="类型"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.version.version"
            label="版本号"
          />

          <div class="col-auto" style="width: 8px" />

          <q-select
            class="col"
            v-model="dialog.version.force.model"
            :options="dialog.version.force.options"
            label="是否强制更新"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input class="col" v-model="dialog.version.link" label="下载地址" />
        </div>

        <div style="height: 8p" />

        <div class="row">
          <q-input
            class="col"
            v-model="dialog.version.dir.oldPath"
            label="旧文件路径"
            placeholder="例: C:\Black Desert\old"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.version.dir.newPath"
            label="新文件路径"
            placeholder="例: C:\Black Desert\new"
          />

          <div class="col-auto" style="width: 8px" />

          <q-input
            class="col"
            v-model="dialog.version.dir.savePath"
            label="保存文件路径"
            placeholder="例: C:\Black Desert\save"
          />
        </div>
      </q-card-section>

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
  version: {
    pagination: {
      page: 1,
      rowsNumber: 0,
      rowsPerPage: 7,
      sortBy: null,
      descending: true,
    } as any,
    filter: '',
    columns: [
      {
        name: 'id',
        label: 'ID',
        field: 'id',
        align: 'center',
        sortable: true,
      },
      {
        name: 'name',
        label: '类别',
        field: 'name',
        align: 'center',
        sortable: true,
      },
      {
        name: 'version',
        label: '版本号',
        field: 'version',
        align: 'center',
        sortable: true,
      },
      {
        name: 'force',
        label: '是否强制更新',
        field: 'force',
        align: 'center',
        sortable: true,
      },
      {
        name: 'link',
        label: '补丁地址',
        field: 'link',
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
        name: 'create_date',
        label: '创建日期',
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
        label: '更新日期',
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
  },
});

const dialog = ref({
  version: {
    show: false,
    id: '',
    name: {
      model: '',
      options: ['app', 'resource'],
    },
    version: '',
    force: {
      model: { label: '否', value: 0 },
      options: [
        { label: '否', value: 0 },
        { label: '是', value: 1 },
      ],
    },
    link: '',
    dir: {
      oldPath: '',
      newPath: '',
      savePath: '',
    },
  },
});

const onView = (val: any) => {
  dialog.value.version.show = true;
  dialog.value.version.id = val.id;
  dialog.value.version.name.model = val.name;
  dialog.value.version.version = val.version;
  dialog.value.version.force.model = val.force;
  dialog.value.version.link = val.link;
  dialog.value.version.dir.oldPath = val.oldPath;
  dialog.value.version.dir.newPath = val.newPath;
  dialog.value.version.dir.savePath = val.savePath;
};

const onRequest = (props: any) => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  let { page, rowsPerPage, sortBy, descending, rowsNumber } = props.pagination;

  table.value.version.rows = [];

  useFetch()
    .post(
      store.backend + '/api/version/info',
      {
        curPage: (page - 1) * rowsPerPage,
        maxPage: rowsPerPage === 0 ? rowsNumber : rowsPerPage,
        sortBy: sortBy === null ? 'version' : sortBy,
        descending: descending,
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      if (resp.data.status != 0) {
        for (let i = 0; i < resp.data.msg.length; ++i) {
          if (resp.data.msg[i].id === '') break;

          table.value.version.rows.push({
            id: resp.data.msg[i].id,
            name: resp.data.msg[i].name,
            version: resp.data.msg[i].version,
            force: resp.data.msg[i].force,
            link: resp.data.msg[i].link,
            oldPath: resp.data.msg[i].oldPath,
            newPath: resp.data.msg[i].newPath,
            savePath: resp.data.msg[i].savePath,
            create_date: resp.data.msg[i].create_date.replace(' +0800', ''),
            create_user: resp.data.msg[i].create_user,
            create_id: resp.data.msg[i].create_id,
            update_date: resp.data.msg[i].update_date.replace(' +0800', ''),
            update_user: resp.data.msg[i].update_user,
            update_id: resp.data.msg[i].update_id,
            delete: resp.data.msg[i].delete,
          });
        }
      } else {
        $q.notify('网络错误，请稍后重试');
      }

      table.value.version.pagination.page = page;
      table.value.version.pagination.rowsPerPage = rowsPerPage;
      table.value.version.pagination.sortBy = sortBy;
      table.value.version.pagination.descending = descending;

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
    .get(store.backend + '/api/version/count', $q.cookies.get('canplay_token'))
    .then((resp) => {
      table.value.version.pagination.rowsNumber = parseInt(resp.data.msg);
      onRequest({ pagination: table.value.version.pagination });

      $q.loading.hide();
      clearTimeout(time);
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onDel = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  dialog.value.version.show = false;

  useFetch()
    .post(
      store.backend + '/api/version/update',
      {
        id: dialog.value.version.id,
        name: dialog.value.version.name.model,
        version: dialog.value.version.version,
        force: dialog.value.version.force.model,
        link: dialog.value.version.link,
        oldPath: dialog.value.version.dir.oldPath,
        newPath: dialog.value.version.dir.newPath,
        savePath: dialog.value.version.dir.savePath,
        update_user: store.user.familyname,
        update_id: store.user.username,
        delete: 1,
      },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      if (resp.data.status === 1) {
        onQuery();
      }

      $q.loading.hide();
      clearTimeout(time);
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

const onSave = () => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  dialog.value.version.show = false;

  if (dialog.value.version.id === '') {
    useFetch()
      .post(
        store.backend + '/api/version/add',
        {
          id: dialog.value.version.id,
          name: dialog.value.version.name.model,
          version: dialog.value.version.version,
          force: dialog.value.version.force.model,
          link: dialog.value.version.link,
          oldPath: dialog.value.version.dir.oldPath,
          newPath: dialog.value.version.dir.newPath,
          savePath: dialog.value.version.dir.savePath,
          create_user: store.user.familyname,
          create_id: store.user.username,
          update_user: store.user.familyname,
          update_id: store.user.username,
        },
        $q.cookies.get('canplay_token')
      )
      .then((resp) => {
        if (resp.data.status === 1) {
          onQuery();
        }

        $q.loading.hide();
        clearTimeout(time);
      })
      .catch(() => {
        $q.notify('网络错误，请稍后重试');
        $q.loading.hide();
        clearTimeout(time);
      });
  } else {
    useFetch()
      .post(
        store.backend + '/api/version/update',
        {
          id: dialog.value.version.id,
          name: dialog.value.version.name.model,
          version: dialog.value.version.version,
          force: dialog.value.version.force.model.value,
          link: dialog.value.version.link,
          oldPath: dialog.value.version.dir.oldPath,
          newPath: dialog.value.version.dir.newPath,
          savePath: dialog.value.version.dir.savePath,
          update_user: store.user.familyname,
          update_id: store.user.username,
          delete: 0,
        },
        $q.cookies.get('canplay_token')
      )
      .then((resp) => {
        if (resp.data.status === 1) {
          onQuery();
        }

        $q.loading.hide();
        clearTimeout(time);
      })
      .catch(() => {
        $q.notify('网络错误，请稍后重试');
        $q.loading.hide();
        clearTimeout(time);
      });
  }
};
</script>
