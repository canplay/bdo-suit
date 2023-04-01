<template>
  <q-dialog v-model="store.dialog.items">
    <q-card>
      <q-card-section>
        <q-table
          title="物品ID列表"
          :rows="table.rows"
          :columns="table.columns"
          row-key="gameids"
          separator="cell"
          v-model:pagination="table.pagination"
          :filter="table.filter"
          @request="onRequest"
        >
          <template v-slot:top-right>
            <q-input
              filled
              dense
              debounce="1000"
              v-model="table.filter"
              label="查找"
            >
              <template v-slot:append>
                <q-icon name="search" />
              </template>
            </q-input>
          </template>
        </q-table>
      </q-card-section>
    </q-card>
  </q-dialog>
</template>

<script setup lang="ts">
import { useStore } from 'src/stores/store';
import { onMounted, ref } from 'vue';
import { QTableProps } from 'quasar';
import { useQuasar } from 'quasar';
import useFetch from './fetch';

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
  columns: [
    {
      name: 'gameid',
      label: 'ID',
      field: 'gameid',
      align: 'center',
      sortable: true,
    },
    {
      name: 'name',
      label: '名称',
      field: 'name',
      align: 'center',
      sortable: true,
    },
  ] as QTableProps['columns'],
  rows: [] as any,
});

const onRequest = (props: any) => {
  let time = setTimeout(() => {
    $q.loading.hide();
    clearTimeout(time);
  }, 120000);

  $q.loading.show();

  const { page, rowsPerPage, sortBy, descending, rowsNumber } =
    props.pagination;
  const filter = props.filter;

  table.value.rows = [];

  useFetch()
    .post(
      store.backend + '/api/items/count',
      { filter: table.value.filter },
      $q.cookies.get('canplay_token')
    )
    .then((resp) => {
      table.value.pagination!.rowsNumber = parseInt(resp.data.msg);

      if (parseInt(resp.data.msg) <= 0) {
        $q.loading.hide();
        clearTimeout(time);
        $q.notify('没有找到物品');
      }

      useFetch()
        .post(
          store.backend + '/api/items/info',
          {
            filter: filter,
            curPage: (page - 1) * rowsPerPage,
            maxPage: rowsPerPage === 0 ? rowsNumber : rowsPerPage,
            sortBy: sortBy === '' ? 'gameid' : sortBy,
            descending: descending,
          },
          $q.cookies.get('canplay_token')
        )
        .then((resp) => {
          $q.loading.hide();
          clearTimeout(time);

          if (resp.data.status != 0) {
            for (let i = 0; i < resp.data.msg.length; ++i) {
              if (resp.data.msg[i].id === '') break;

              table.value.rows.push({
                gameid: resp.data.msg[i].gameid,
                name: resp.data.msg[i].name,
              });
            }
          } else {
            $q.notify('网络错误，请稍后重试');
          }

          table.value.pagination!.page = page;
          table.value.pagination!.rowsPerPage = rowsPerPage;
          table.value.pagination!.sortBy = sortBy;
          table.value.pagination!.descending = descending;
        })
        .catch(() => {
          $q.notify('网络错误，请稍后重试');
          $q.loading.hide();
          clearTimeout(time);
        });
    })
    .catch(() => {
      $q.notify('网络错误，请稍后重试');
      $q.loading.hide();
      clearTimeout(time);
    });
};

onMounted(() => {
  onRequest({ pagination: table.value.pagination });
});
</script>
