create table DeveloperRole
(
    id   int generated always as identity primary key not null,
    role text                                         not null
);

create table DeveloperInfo
(
    id   int        generated always as identity primary key not null,
    name text                                                not null,
    age  int                                                 not null,
    role int                                                 not null,
    foreign key (role) references DeveloperRole (id) on delete cascade
);

create table Project
(
    id          int generated always as identity primary key not null,
    name        text                                         not null,
    description text
);

create table Developer
(
    id       int         generated always as identity primary key not null,
    dev_info int                                                  not null,
    project  int                                                  not null,
    foreign key (dev_info) references DeveloperInfo (id) on delete cascade,
    foreign key (project) references Project (id)        on delete cascade
);

create table ResourceType
(
    id   int generated always as identity primary key not null,
    name text                                         not null
);

create table Resource
(
    id          int generated always as identity primary key not null,
    name        text                                         not null,
    description text,
    size        int                                          not null,
    checksum    uuid                                         not null,
    type        int                                          not null,
    data        date                                         not null, --поменял с oid
    foreign key (type) references ResourceType (id)  on delete cascade
);

create table Dependency
(
    id                  int     generated always as identity primary key not null,
    requesting_resource int                                              not null,
    required_resource   int                                              not null,
    foreign key (requesting_resource) references Resource (id)  on delete cascade,
    foreign key (required_resource) references Resource (id)    on delete cascade
);

create table Commits
(
    id       int   generated always as identity primary key not null,
    dev      int                                            not null,
    resource int                                            not null,
    date     timestamp                                      not null,
    message  text,
    foreign key (dev) references Developer (id)    on delete cascade,
    foreign key (resource) references Resource (id) on delete cascade
);

-- create user admin with

insert into Project(name, description) values ('PogProject', 'kekW');

insert into DeveloperRole(role) values ('CharacterDesigner');
insert into DeveloperRole(role) values ('Animator');
insert into DeveloperRole(role) values ('LevelDesigner');

insert into DeveloperInfo(name, age, role) values ('Dev1', 20, (select id from DeveloperRole where role = 'Animator'));
insert into DeveloperInfo(name, age, role) values ('Dev2', 30, (select id from DeveloperRole where role = 'Animator'));
insert into DeveloperInfo(name, age, role) values ('Dev3', 40, (select id from DeveloperRole where role = 'LevelDesigner'));

insert into Developer(dev_info, project) values ((select id from DeveloperInfo where name = 'Dev1'), (select id from Project where name = 'PogProject'));
insert into Developer(dev_info, project) values ((select id from DeveloperInfo where name = 'Dev2'), (select id from Project where name = 'PogProject'));
insert into Developer(dev_info, project) values ((select id from DeveloperInfo where name = 'Dev3'), (select id from Project where name = 'PogProject'));

insert into ResourceType(name) values ('Texture');
insert into ResourceType(name) values ('Material');
insert into ResourceType(name) values ('Sprite');
insert into ResourceType(name) values ('Mesh');
insert into ResourceType(name) values ('Level');

insert into Resource(name, description, size, checksum, type, data) values ('Level1', '', 12, 'A0EEBC99-9C0B-4EF8-BB6D-6BB9BD380A11', (select id from ResourceType where name = 'Level'), '2000-01-01');
insert into Resource(name, description, size, checksum, type, data) values ('Material1', '', 12, 'A0EAAC89-9C0B-4EF8-BB6D-6BB9BD380A11', (select id from ResourceType where name = 'Material'), '2010-10-10');

-- insert into meshes(file_name, date_of_creation, date_of_last_edit, creator, description, file_size,
--                    checksum)
-- values ('test', '2000-1-1', '2020-1-1', 'pog', 'pogger', 3228, 'A0EEBC99-9C0B-4EF8-BB6D-6BB9BD380A11');

--каждому  foreign key добавил атрибут удаления

--роли, роль админа вроде как поумолчанию есть, иначе
--create role Admin superuser;
create  role hight_order_dev;
create  role common_devs;
grant select,insert,update on Project to hight_order_dev;
grant select,insert on Resource,Commits,Dependency to common_devs;
grant select,insert on Resource,Commits,Dependency to  hight_order_dev;
/* insert ты везде почти сам сделал, а delete я предпочел делать через name или подобные атрибуты,а не черз id
потому что, при удалении записии старое id не освобождается*/
--ADD/DELETE

--add/delete project
delete from Project where  name ='PogProject';
--delete from Project where id  = 1;

--add/delete dev_info
delete from DeveloperInfo where name = 'Dev1';
--delete from DeveloperInfo where id = 1;

--add/delete role
delete from DeveloperRole where  role ='Animator';
--delete from DeveloperRole where id = 1;

--add/delete commits
insert into Commits(dev, resource, date, message) values ((select id from Developer where dev_info =
(select id from DeveloperInfo where name = 'Dev1')),(select id from Resource where name = 'Level1'),'2010-10-10','Added saving');

--delete from Commits where dev = (select id from Developer where dev_info = (select id from DeveloperInfo where name = 'Dev1'));
--логичнее всего удалять зная Dev'а, ресуры и описания, т.к. у одного дева может быть больше 1 коммита
delete from Commits where dev = (select id from Developer where dev_info = (select id from DeveloperInfo where name = 'Dev1'))
and resource = (select id from Resource where name = 'Level1') and message ='Added saving' ;
--delete from Commit where id = 1;

--add/delete ResourceType
delete from ResourceType where name = 'Mesh';
--delete from ResourceType where id = 1;

--add/delete Dependency
insert into Dependency(requesting_resource, required_resource) values ((select id from Resource where name = 'Level1'),(select id from Resource where name = 'Material1'));
delete from Dependency where requesting_resource =(select id from Resource where name = 'Level1') and required_resource = (select id from Resource where name = 'Material1');
--delete from Dependency where id = 1;

--add/delete Resource
delete from Resource where name ='Level1';
--delete from Resousre where id = 1;


--GET (select)
--get resource types
select name from ResourceType;

--get resources of type
select * from Resource where type = (select id  from ResourceType where name = 'Level');

--get resource commit history 3 варианта
select * from Commits where resource = (select id from Resource where name = 'Level1' );
--or
select * from Commits where resource = (select id from Resource where type = (select id from ResourceType where name ='Level'));
--or
select commits.id,dev,Commits.resource,name as name_of_resourse,date,message
from Commits join Resource on Commits.resource = Resource.id;

--get resource info
select * from Resource;

--get commit info;
select * from  Commits;

--get resource dependency
select * from Dependency;
--or
select  Dependency.id,Requesting.name,Required.name from Dependency join Resource Requesting on
Dependency.requesting_resource = Requesting.id join Resource Required on Dependency.required_resource=Required.id;

--get dev_info
select * from DeveloperInfo;
--or
select DeveloperInfo.id,Dr.role as Designation,name, age from DeveloperInfo join DeveloperRole DR on DR.id = DeveloperInfo.role;


--Alter
--resource commit history
update Commits set dev = 8, resource =2, date = '2012-10-10', message = 'Add new material' where id = 4;
--or
update  Commits set dev = (select id from Developer where dev_info =(select id from DeveloperInfo where name = 'Dev1')),
resource = (select id from Resource where name = 'Texture1'),date = '2012-10-10', message = 'Add new material' where id = 4;

--resource info
update Resource set name = 'Texture1', description = '', size = 12,
checksum = 'a0eebc99-9c0b-4ef8-bb6d-6bb9bd380a11', type =1, data = '2015-10-01' where id = 2  ;
--or
update Resource set name = 'Texture1',description ='',size =12, checksum='a0eebc99-9c0b-4ef8-bb6d-6bb9bd380a11',
type = (select id from ResourceType where name = 'Texture'), data ='2015-10-02' where id =2;


--dev_info
update DeveloperInfo set name = 'Dev4', age = 25, role =1 where  id = 3;
--or
update DeveloperInfo set name = 'Dev4', age = 25, role =(select id from DeveloperRole where role = 'Animator') where  id = 3;

--Project
update Project set name = 'OmegaLuLProject', description = '4head' where id = 2;

--dependency
update  Dependency set requesting_resource = 3, required_resource =2 where id = 1;
--or
update Dependency set requesting_resource = (select id from Resource where name = 'Level1'),
required_resource = (select id from Resource where name = 'Texture1') where id = 4;



--Notifications
listen mymessage;
create or replace function resource_notify()
  returns trigger as
$TRIGGER_R$
    begin
        notify mymessage, 'Resource update';
        return NULL;
    end ;
$TRIGGER_R$
  language plpgsql volatile
  cost 100;
alter function resource_notify() owner to postgres;

create trigger trigger_resource
  after insert or update or delete
  on Commits
  for each row
  execute procedure resource_notify();


create or replace function commits_notify()
  returns trigger as
$TRIGGER_C$
    begin
        notify mymessage, 'Resource update';
        return NULL;
    end ;
$TRIGGER_C$
  language plpgsql volatile
  cost 100;
alter function commits_notify() owner to postgres;

create trigger trigger_commits
  after insert or update or delete
  on Commits
  for each row
  execute procedure commits_notify();

